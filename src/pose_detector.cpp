#include <trifinger_object_tracking/pose_detector.hpp>

#include <float.h>
#include <math.h>
#include <iostream>
#include <random>
#include <thread>

#include <trifinger_object_tracking/scoped_timer.hpp>

namespace trifinger_object_tracking
{
// TODO: use quaternion instead of matrix (more efficient).
cv::Mat getPoseMatrix(cv::Point3f orientation, cv::Point3f position)
{
    cv::Vec3f rvec(orientation.x, orientation.y, orientation.z);
    cv::Vec3f tvec(position.x, position.y, position.z);

    // TODO keep fixed-size Mat4
    return cv::Mat(cv::Affine3f(rvec, tvec).matrix);
}

PoseDetector::PoseDetector(const CubeModel &cube_model,
                           const std::array<trifinger_cameras::CameraParameters,
                                            N_CAMERAS> &camera_parameters)
    : cube_model_(cube_model)
{
    // convert camera parameters to cv types
    for (int i = 0; i < N_CAMERAS; i++)
    {
        cv::eigen2cv(camera_parameters[i].camera_matrix, camera_matrices_[i]);
        cv::eigen2cv(camera_parameters[i].distortion_coefficients,
                     distortion_coeffs_[i]);

        Eigen::Matrix3d rotmat_eigen =
            camera_parameters[i].tf_world_to_camera.topLeftCorner<3, 3>();
        cv::Mat rotmat_cv;
        cv::eigen2cv(rotmat_eigen, rotmat_cv);
        cv::Rodrigues(rotmat_cv, camera_orientations_[i]);

        Eigen::Vector3d tvec_eigen =
            camera_parameters[i].tf_world_to_camera.topRightCorner<3, 1>();
        cv::eigen2cv(tvec_eigen, camera_translations_[i]);
    }

    // unfortunately, there is no real const cv::Mat, so we cannot wrap it
    // around the const array but need to copy the data
    corners_at_origin_in_world_frame_ = cv::Mat(8, 4, CV_32F);
    std::memcpy(corners_at_origin_in_world_frame_.data,
                cube_model_.corners_at_origin_in_world_frame,
                corners_at_origin_in_world_frame_.total() * sizeof(float));

    reference_vector_normals_ = cv::Mat(3, 6, CV_32F);
    std::memcpy(reference_vector_normals_.data,
                cube_model_.face_normal_vectors,
                reference_vector_normals_.total() * sizeof(float));

    // Setting the bounds for pose estimation
    position_.lower_bound = cv::Point3f(-0.25, -0.25, 0);
    position_.upper_bound = cv::Point3f(0.25, 0.25, 0.25);

    position_.mean = (position_.upper_bound + position_.lower_bound) / 2.0;
    position_.variance =
        power((position_.lower_bound - position_.upper_bound) / 4.0, 2);

    orientation_.lower_bound =
        cv::Point3f(-3.14159265, -3.14159265, -3.14159265);
    orientation_.upper_bound = cv::Point3f(3.14159265, 3.14159265, 3.14159265);
    orientation_.mean =
        (orientation_.upper_bound + orientation_.lower_bound) / 2.0;
    orientation_.variance =
        power((orientation_.lower_bound - orientation_.upper_bound) / 4.0, 2);
}

cv::Point3f PoseDetector::power(cv::Point3f p, float n)
{
    p.x = pow(p.x, n);
    p.y = pow(p.y, n);
    p.z = pow(p.z, n);
    return p;
}

std::vector<cv::Point3f> PoseDetector::random_normal(
    cv::Point3f mean, cv::Point3f var, int rows, int cols, std::string clip_for)
{
    std::vector<cv::Point3f> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    //    std::default_random_engine gen;
    std::normal_distribution<float> d1{mean.x, var.x};
    std::normal_distribution<float> d2{mean.y, var.y};
    std::normal_distribution<float> d3{mean.z, var.z};
    float x, y, z;
    for (int r = 0; r < rows; r++)
    {
        x = d1(gen);
        y = d2(gen);
        z = d3(gen);

        // clipping between lower and upper bounds
        if (clip_for == "position")
        {
            x = std::max(position_.lower_bound.x,
                         std::min(x, position_.upper_bound.x));
            y = std::max(position_.lower_bound.y,
                         std::min(y, position_.upper_bound.y));
            z = std::max(position_.lower_bound.z,
                         std::min(z, position_.upper_bound.z));
        }
        if (clip_for == "orientation")
        {
            x = std::max(orientation_.lower_bound.x,
                         std::min(x, orientation_.upper_bound.x));
            y = std::max(orientation_.lower_bound.y,
                         std::min(y, orientation_.upper_bound.y));
            z = std::max(orientation_.lower_bound.z,
                         std::min(z, orientation_.upper_bound.z));
        }

        data.push_back(cv::Point3f(x, y, z));
    }
    return data;
}

std::vector<cv::Point3f> PoseDetector::random_uniform(cv::Point3f lower_bound,
                                                      cv::Point3f upper_bound,
                                                      int rows,
                                                      int cols)
{
    std::vector<cv::Point3f> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    //    std::default_random_engine gen;
    std::uniform_real_distribution<float> d1{lower_bound.x, upper_bound.x};
    std::uniform_real_distribution<float> d2{lower_bound.y, upper_bound.y};
    std::uniform_real_distribution<float> d3{lower_bound.z, upper_bound.z};
    float x, y, z;
    for (int r = 0; r < rows; r++)
    {
        x = d1(gen);
        y = d2(gen);
        z = d3(gen);

        data.push_back(cv::Point3f(x, y, z));
    }
    return data;
}

std::vector<cv::Point3f> PoseDetector::sample_random_so3_rotvecs(
    int number_of_particles)
{
    std::vector<cv::Point3f> data;
    for (int r = 0; r < number_of_particles; r++)
    {
        Eigen::Quaterniond quat = Eigen::Quaterniond::UnitRandom();
        Eigen::Matrix3d R = quat.toRotationMatrix();
        cv::Mat rotation_matrix, rotation_vector;
        cv::eigen2cv(R, rotation_matrix);
        cv::Rodrigues(rotation_matrix, rotation_vector);
        data.push_back(cv::Point3f(rotation_vector));
    }
    return data;
}

cv::Mat PoseDetector::_cost_of_out_of_bounds_projection(
    const std::array<cv::Mat, N_CAMERAS> &projected_points)
{
    ScopedTimer timer("PoseDetector/_cost_of_out_of_bounds_projection");

    auto s = projected_points[0].size();
    int number_of_particles = s.height;
    cv::Mat error(number_of_particles, 1, CV_32FC1, cv::Scalar(0));
    int width = 720;
    int height = 540;
    int threshold = 30;

    for (int i = 0; i < N_CAMERAS; i++)
    {
        for (int j = 0; j < number_of_particles; j++)
        {
            bool flag1 = false;
            bool flag2 = false;
            bool flag3 = false;
            bool flag4 = false;

            for (int k = 0; k < 8; k++)
            {
                auto element = projected_points[i].at<cv::Vec2f>(j, k);
                if (0 - threshold > element[0])
                {
                    flag1 = true;
                }
                if (element[0] > width + threshold)
                {
                    flag2 = true;
                }
                if (0 - threshold > element[1])
                {
                    flag3 = true;
                }
                if (element[1] > height + threshold)
                {
                    flag4 = true;
                }
            }
            if (flag1 || flag2)
            {
                error.at<float>(j, 0) = FLT_MAX;
            }

            if (flag3 || flag4)
            {
                error.at<float>(j, 0) = FLT_MAX;
            }
        }
    }
    return error;
}

cv::Mat PoseDetector::_get_face_normals_cost(
    const std::vector<cv::Mat> &object_pose_matrices)
{
    ScopedTimer timer("PoseDetector/_get_face_normals_cost");

    int number_of_particles = object_pose_matrices.size();

    // for each particle a 3x6 matrix with column-wise face normal vectors
    std::vector<cv::Mat> face_normal_vectors;  // number_of_particles x 3 x 6
    face_normal_vectors.reserve(number_of_particles);
    for (int i = 0; i < number_of_particles; i++)
    {
        cv::Mat v_face;
        v_face = object_pose_matrices[i].colRange(0, 3).rowRange(0, 3) *
                 reference_vector_normals_;  // 3x6
        face_normal_vectors.push_back(v_face);
    }

    cv::Mat error(number_of_particles, 1, CV_32FC1, cv::Scalar(0));

    for (int i = 0; i < N_CAMERAS; i++)
    {
        // v_cam_to_cube --> number_of_particles x 3
        std::vector<cv::Vec3f> v_cam_to_cube;
        cv::Vec3f a(pos_cams_w_frame_[i].rowRange(0, 3).col(3));
        for (int j = 0; j < number_of_particles; j++)
        {
            cv::Vec3f b(object_pose_matrices[j].rowRange(0, 3).col(3));
            cv::Vec3f c = a - b;
            float norm = cv::norm(c);
            c = c / norm;
            v_cam_to_cube.push_back(c);
        }

        std::set<FaceColor> color_set;
        for (auto &it : lines_[i])
        {
            color_set.insert(it.first.first);
            color_set.insert(it.first.second);
        }

        for (auto &color : color_set)
        {
            for (int j = 0; j < number_of_particles; j++)
            {
                float angle = 0;
                const int normal_vector_index =
                    cube_model_.map_color_to_normal_index[color];

                cv::Vec3f v_a = face_normal_vectors[j].col(normal_vector_index);
                cv::Vec3f v_b(v_cam_to_cube[j]);
                float dot_product = v_a.dot(v_b);

                angle = std::abs(std::acos(dot_product));

                angle -= M_PI / 2;
                if (angle < 0)
                {
                    angle = 0;
                }

                error.at<float>(j, 0) += angle;
            }
        }
    }
    return error;
}

std::vector<float> PoseDetector::cost_function(
    std::vector<cv::Point3f> proposed_translation,
    std::vector<cv::Point3f> proposed_orientation)
{
    ScopedTimer timer("PoseDetector/cost_function");

    int number_of_particles = proposed_translation.size();
    std::vector<cv::Mat> pose;
    cv::Mat proposed_new_cube_pts_w(
        number_of_particles, 8, CV_32FC3, cv::Scalar(0, 0, 0));

    // for each particle compute cube corners at the given pose
    pose.reserve(number_of_particles);
    for (int i = 0; i < number_of_particles; i++)
    {
        // initialization of pose
        cv::Mat rotation_matrix =
            getPoseMatrix(proposed_orientation[i], proposed_translation[i]);

        pose.push_back(rotation_matrix);
        cv::Mat new_pt =
            rotation_matrix * corners_at_origin_in_world_frame_.t();

        new_pt = new_pt.t();  // 8x4
        for (int j = 0; j < new_pt.rows; j++)
        {
            // 8x3
            proposed_new_cube_pts_w.at<cv::Vec3f>(i, j) =
                cv::Vec3f(new_pt.at<float>(j, 0),
                          new_pt.at<float>(j, 1),
                          new_pt.at<float>(j, 2));
        }
    }

    proposed_new_cube_pts_w =
        proposed_new_cube_pts_w.reshape(3, number_of_particles * 8);

    // project the cube corners of the particles to the images
    std::array<cv::Mat, N_CAMERAS> projected_points;
    for (int i = 0; i < N_CAMERAS; i++)
    {
        // range (r_vecs)
        cv::Mat imgpoints(number_of_particles * 8, 2, CV_32FC1, cv::Scalar(0));
        cv::projectPoints(proposed_new_cube_pts_w,
                          camera_orientations_[i],
                          camera_translations_[i],
                          camera_matrices_[i],
                          distortion_coeffs_[i],
                          imgpoints);

        projected_points[i] = imgpoints.reshape(2, number_of_particles);
    }

    // Error matrix initialisation
    cv::Mat error;
    constexpr float FACE_NORMALS_SCALING_FACTOR = 500.0;
    error = FACE_NORMALS_SCALING_FACTOR * _get_face_normals_cost(pose);
    error = error + _cost_of_out_of_bounds_projection(projected_points);

    for (int i = 0; i < N_CAMERAS; i++)
    {
        // range (r_vecs)
        cv::Mat imgpoints_reshaped = projected_points[i];
        for (auto &line_it : lines_[i])
        {
            auto points_lying = cube_model_.object_model_.at(line_it.first);
            Line line = line_it.second;

            cv::Mat points_on_edge(
                number_of_particles, 2, CV_32FC2, cv::Scalar(0, 0));

            imgpoints_reshaped.col(points_lying.first)
                .copyTo(points_on_edge.col(0));
            imgpoints_reshaped.col(points_lying.second)
                .copyTo(points_on_edge.col(1));
            cv::Mat distance;  // 2Nx1
            cv::Mat ch1, ch2;
            std::vector<cv::Mat> channels(2);
            split(points_on_edge, channels);
            absdiff(line.a * channels[1], channels[0] - line.b, distance);
            distance = distance * (1 / (pow(pow(line.a, 2) + 1, 0.5)));
            cv::Mat reduced_error(
                number_of_particles, 1, CV_32FC1, cv::Scalar(0));
            cv::reduce(distance, reduced_error, 1, CV_REDUCE_SUM);
            error = error + reduced_error;
        }
    }
    return error;
}

void PoseDetector::initialise_pos_cams_w_frame()
{
    ScopedTimer timer("PoseDetector/initialise_pos_cams_w_frame");

    for (int i = 0; i < N_CAMERAS; i++)
    {
        cv::Mat pos_cam = getPoseMatrix(cv::Point3f(camera_orientations_[i]),
                                        cv::Point3f(camera_translations_[i]));
        pos_cam = pos_cam.inv();
        pos_cams_w_frame_.push_back(pos_cam);
    }
}

void PoseDetector::cross_entropy_method()
{
    ScopedTimer timer("PoseDetector/cross_entropy_method");

    int max_iterations = 30;
    int number_of_particles = 1000;
    int elites = 100;
    float alpha = 0.3;
    float eps = 5.0;
    best_cost_ = FLT_MAX;
    std::vector<float> costs;

    if (continuous_estimation_ == false && initialisation_phase_ == false)
    {
        initialisation_phase_ = true;
    }

    // FIXME this is probably static and should be done in c'tor
    initialise_pos_cams_w_frame();

    for (int i = 0; i < max_iterations && best_cost_ > eps; i++)
    {
        std::vector<cv::Point3f> sample_p;
        std::vector<cv::Point3f> sample_o;
        if (initialisation_phase_ == true)
        {
            // TODO: fix the following for initialisation phase
            sample_p = random_uniform(position_.lower_bound,
                                      position_.upper_bound,
                                      number_of_particles * 10,
                                      3);

            sample_o = sample_random_so3_rotvecs(number_of_particles * 10);
        }
        else
        {
            // TODO: reduce the number_of_particles from 10k to 1k
            sample_p = random_normal(position_.mean,
                                     power(position_.variance, 0.5),
                                     number_of_particles,
                                     3,
                                     "position");

            sample_o = random_normal(orientation_.mean,
                                     power(orientation_.variance, 0.5),
                                     number_of_particles,
                                     3,
                                     "orientation");
        }
        costs = cost_function(sample_p, sample_o);

        std::vector<float> sorted_costs = costs;
        sort(sorted_costs.begin(), sorted_costs.end());
        sorted_costs.resize(elites);

        if (sorted_costs[0] < best_cost_)
        {
            best_cost_ = sorted_costs[0];
            int idx =
                find(costs.begin(), costs.end(), best_cost_) - costs.begin();
            best_position_ = sample_p[idx];
            best_orientation_ = sample_o[idx];
        }

        std::vector<cv::Point3f> elites_p;
        std::vector<cv::Point3f> elites_o;
        for (auto &it : sorted_costs)
        {
            int idx = find(costs.begin(), costs.end(), it) - costs.begin();
            elites_p.push_back(sample_p[idx]);
            elites_o.push_back(sample_o[idx]);
        }

        cv::Point3f new_mean_position = mean(elites_p);
        cv::Point3f new_mean_orientation = mean(elites_o);
        cv::Point3f new_var_position = var(elites_p);
        cv::Point3f new_var_orientation = var(elites_o);

        if (initialisation_phase_ == true)
        {
            position_.mean = new_mean_position;
            orientation_.mean = new_mean_orientation;
            position_.variance = new_var_position;
            orientation_.variance = new_var_orientation;
            initialisation_phase_ = false;
        }
        else
        {
            position_.mean =
                (alpha * position_.mean) + ((1 - alpha) * new_mean_position);
            orientation_.mean = (alpha * orientation_.mean) +
                                ((1 - alpha) * new_mean_orientation);
            position_.variance =
                (alpha * position_.variance) + ((1 - alpha) * new_var_position);
            orientation_.variance = (alpha * orientation_.variance) +
                                    ((1 - alpha) * new_var_orientation);
        }
    }
    std::cout << "Best cost: " << best_cost_ << std::endl;
    if (continuous_estimation_ == true)
    {
        position_.mean = best_position_;
        orientation_.mean = best_orientation_;
    }
}

cv::Point3f PoseDetector::mean(const std::vector<cv::Point3f> &points)
{
    cv::Point3f p(0., 0., 0.);
    for (const auto &it : points)
    {
        p += it;
    }
    float N = 1.0 / points.size();
    p = p * N;
    return p;
}

cv::Point3f PoseDetector::var(const std::vector<cv::Point3f> &points)
{
    cv::Point3f m = mean(points);
    cv::Point3f p(0., 0., 0.);
    for (const auto &it : points)
    {
        p += power((it - m), 2);
    }
    float N = 1.0 / (points.size());
    p = p * N;
    return p;
}

Pose PoseDetector::find_pose(
    const std::array<ColorEdgeLineList, N_CAMERAS> &lines)
{
    ScopedTimer timer("PoseDetector/find_pose");

    // FIXME this is bad design
    lines_ = lines;

    cross_entropy_method();  // calculates mean_position and mean_orientation

    if (best_cost_ > 50)
    {
        initialisation_phase_ = true;
        cross_entropy_method();
        if (best_cost_ > 50)
        {
            initialisation_phase_ = true;
        }
    }

    return Pose(position_.mean, orientation_.mean);
}

std::vector<std::vector<cv::Point2f>> PoseDetector::get_projected_points() const
{
    std::vector<std::vector<cv::Point2f>> projected_points;

    cv::Mat pose = getPoseMatrix(orientation_.mean, position_.mean);

    cv::Mat proposed_new_cube_pts_w =
        pose * corners_at_origin_in_world_frame_.t();
    proposed_new_cube_pts_w = proposed_new_cube_pts_w.t();  // 8x4
    proposed_new_cube_pts_w = proposed_new_cube_pts_w.colRange(
        0, proposed_new_cube_pts_w.cols - 1);  // 8x3

    std::vector<cv::Point3f> _new_pts;
    for (int i = 0; i < proposed_new_cube_pts_w.rows; i++)
    {
        _new_pts.push_back(
            cv::Point3f(proposed_new_cube_pts_w.at<float>(i, 0),
                        proposed_new_cube_pts_w.at<float>(i, 1),
                        proposed_new_cube_pts_w.at<float>(i, 2)));
    }

    for (int i = 0; i < N_CAMERAS; i++)
    {  // range (r_vecs)
        std::vector<cv::Point2f> imgpoints;
        cv::projectPoints(_new_pts,
                          camera_orientations_[i],
                          camera_translations_[i],
                          camera_matrices_[i],
                          distortion_coeffs_[i],
                          imgpoints);
        projected_points.push_back(imgpoints);
    }

    return projected_points;
}

}  // namespace trifinger_object_tracking
