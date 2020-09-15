#include <float.h>
#include <math.h>

#include <iostream>
#include <random>
#include <thread>
#include <trifinger_object_tracking/pose.hpp>

namespace trifinger_object_tracking
{
Pose::Pose(const CubeModel &cube_model,
           const std::array<std::map<ColorPair, Line>, 3> &lines)
    : cube_model_(cube_model), lines_(lines)
{
    float camera_matrix[3][3] = {{589.607902244274, 0.0, 366.49661815699994},
                                 {0.0, 590.1790734214388, 297.98736394590526},
                                 {0.0, 0.0, 1.0}};

    float distortion_coeffs[5][1] = {{-0.2489693848298703},
                                     {0.13435384837763378},
                                     {0.0003204379158765275},
                                     {-0.00036140843150739765},
                                     {-0.06579839150308762}};

    float rotation_matrix[3][3] = {
        {1.0890998989916354,
         2.614833277418142,
         1.0605210499895859},  // 1st column is r60, 2nd is r180, 3rd is r300
        {2.5172012639946733, -0.026669213558675804, -2.5351740094307447},
        {-0.8452495044215251, -0.01769147496035694, 0.9169399036537441}};

    float translation_matrix[3][3] = {
        {-0.016788995687316167,
         -0.001077263937284312,
         -0.0030656937101139854},  // 1st column is t60, 2nd is t180, 3rd is
                                   // t300
        {-0.02269247299737612, -0.015886005467183863, -0.022204248661934205},
        {0.5454465438837742, 0.5365514015146118, 0.5282896793079157}};

    float reference_center_Point_3d[8][4] = {{0.0326, -0.0326, 0.0652, 1},
                                             {-0.0326, -0.0326, 0.0652, 1},
                                             {0.0326, 0.0326, 0.0652, 1},
                                             {-0.0326, 0.0326, 0.0652, 1},
                                             {0.0326, -0.0326, 0, 1},
                                             {-0.0326, -0.0326, 0, 1},
                                             {0.0326, 0.0326, 0, 1},
                                             {-0.0326, 0.0326, 0, 1}};

    float reference_vector_normals[3][6] = {
        {0, 0, 1, -1, 0, 0}, {0, 0, 0, 0, 1, -1}, {1, -1, 0, 0, 0, 0}};

    camera_matrix_ = cv::Mat(3, 3, CV_32F, &camera_matrix).clone();
    distortion_coeffs_ = cv::Mat(5, 1, CV_32F, &distortion_coeffs).clone();
    rotation_matrix_ = cv::Mat(3, 3, CV_32F, &rotation_matrix).clone();
    translation_matrix_ = cv::Mat(3, 3, CV_32F, &translation_matrix).clone();
    reference_center_Point_3d_ =
        cv::Mat(8, 4, CV_32F, &reference_center_Point_3d).clone();
    reference_vector_normals_ =
        cv::Mat(3, 6, CV_32F, &reference_vector_normals).clone();

    face_normals_v_[FaceColor::YELLOW] = {0};
    face_normals_v_[FaceColor::RED] = {1};
    face_normals_v_[FaceColor::MAGENTA] = {2};
    face_normals_v_[FaceColor::GREEN] = {3};
    face_normals_v_[FaceColor::BLUE] = {4};
    face_normals_v_[FaceColor::CYAN] = {5};

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

cv::Point3f Pose::power(cv::Point3f p, float n)
{
    p.x = pow(p.x, n);
    p.y = pow(p.y, n);
    p.z = pow(p.z, n);
    return p;
}

std::vector<cv::Point3f> Pose::random_normal(
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

std::vector<cv::Point3f> Pose::random_uniform(cv::Point3f lower_bound,
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

cv::Mat Pose::getPoseMatrix(cv::Point3f orientation, cv::Point3f position)
{
    cv::Mat rotation_matrix;
    cv::Rodrigues(cv::Mat(orientation), rotation_matrix);
    cv::Mat translation =
        (cv::Mat_<float>(3, 1) << position.x, position.y, position.z);
    cv::hconcat(rotation_matrix, translation, rotation_matrix);
    float temp[4] = {0, 0, 0, 1.0};
    rotation_matrix.push_back(cv::Mat(1, 4, CV_32F, &temp));
    return rotation_matrix;  // 4x4
}

std::vector<cv::Point3f> Pose::sample_random_so3_rotvecs(
    int number_of_particles)
{ /* https://stackoverflow.com/questions/38844493/transforming-quaternion-to-camera-rotation-matrix-opencv
   * 1. convert quat to axis-angle vectors
   * 2. multiply angle by axis to get Rodrigues angels
   * 3. use cv::Rodrigues(rod_angles) to get rotation_matrix
   */

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

cv::Mat Pose::_cost_of_out_of_bounds_projection(
    std::vector<cv::Mat> projected_points)
{
    auto s = projected_points[0].size();
    int number_of_particles = s.height;
    cv::Mat error(number_of_particles, 1, CV_32FC1, cv::Scalar(0));
    int width = 720;
    int height = 540;
    int threshold = 30;

    for (int i = 0; i < 3; i++)
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

cv::Mat Pose::_get_face_normals_cost(
    std::vector<cv::Mat> proposed_orientation_matrices)
{
    std::vector<cv::Mat> v_faces;  // number_of_particles x 3 x 6
    int number_of_particles = proposed_orientation_matrices.size();
    for (int i = 0; i < number_of_particles; i++)
    {
        cv::Mat v_face;
        v_face =
            proposed_orientation_matrices[i].colRange(0, 3).rowRange(0, 3) *
            reference_vector_normals_;  // 3x6
        v_faces.push_back(v_face);
    }
    cv::Mat error(number_of_particles, 1, CV_32FC1, cv::Scalar(0));

    for (int i = 0; i < 3; i++)
    {
        // v_cam_to_cube --> number_of_particles x 3
        std::vector<cv::Point3f> v_cam_to_cube;
        cv::Point3f a(pos_cams_w_frame_[i].rowRange(0, 3).col(3));
        for (int j = 0; j < number_of_particles; j++)
        {
            cv::Point3f b(
                proposed_orientation_matrices[j].rowRange(0, 3).col(3));
            cv::Point3f c = a - b;
            float norm = cv::norm(c);
            c = c / norm;
            v_cam_to_cube.push_back(c);
        }
        auto lines = lines_[i];
        std::set<FaceColor> color_set;
        for (auto &it : lines)
        {
            color_set.insert(it.first.first);
            color_set.insert(it.first.second);
        }

        for (auto &color : color_set)
        {
            cv::Mat angle_between_face_and_cam;
            for (int j = 0; j < number_of_particles; j++)
            {
                float angle = 0;
                cv::Mat mat_a = v_faces[j].col(face_normals_v_[color]);
                cv::Mat mat_b(v_cam_to_cube[j]);
                cv::Mat product = mat_a.mul(mat_b);
                cv::Mat summed;
                cv::reduce(product, summed, 0, CV_REDUCE_SUM);
                float element_wise_product_and_sum = summed.at<float>(0, 0);
                angle = std::abs(std::acos(element_wise_product_and_sum));

                int debug = 0;
                if (debug == 1)
                {
                    std::cout << "Mat_a " << mat_a << std::endl;
                    std::cout << "Mat_b " << mat_b << std::endl;
                    std::cout << "product " << product << std::endl;
                    std::cout << "summed " << summed << std::endl;
                    std::cout << "element_wise_product_and_sum "
                              << element_wise_product_and_sum << std::endl;
                    std::cout << "angle " << angle << std::endl;
                }

                angle -= M_PI / 2;
                if (angle < 0)
                {
                    angle = 0;
                }
                angle_between_face_and_cam.push_back(
                    cv::Mat(1, 1, CV_32F, &angle));
            }
            error += angle_between_face_and_cam;
        }
    }
    return error;
}

std::vector<float> Pose::cost_function(
    std::vector<cv::Point3f> proposed_translation,
    std::vector<cv::Point3f> proposed_orientation)
{
    int number_of_particles = proposed_translation.size();
    std::vector<cv::Mat> pose;
    // cv::Mat proposed_new_cube_pts_w;
    cv::Mat proposed_new_cube_pts_w(
        number_of_particles, 8, CV_32FC3, cv::Scalar(0, 0, 0));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < number_of_particles; i++)
    {  // initialization of pose
        cv::Mat rotation_matrix =
            getPoseMatrix(proposed_orientation[i], proposed_translation[i]);
        pose.push_back(rotation_matrix);
        cv::Mat new_pt = rotation_matrix * reference_center_Point_3d_.t();
        new_pt = new_pt.t();  // 8x4
        for (int j = 0; j < new_pt.rows; j++)
        {  // 8x3
            proposed_new_cube_pts_w.at<cv::Vec3f>(i, j) =
                cv::Vec3f(new_pt.at<float>(j, 0),
                          new_pt.at<float>(j, 1),
                          new_pt.at<float>(j, 2));
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    // std::cout << "CEM creating 3d proposed points took "
    //          << std::chrono::duration_cast<std::chrono::milliseconds>(finish
    //          -
    //                                                                   start)
    //                 .count()
    //          << " milliseconds\n";
    proposed_new_cube_pts_w =
        proposed_new_cube_pts_w.reshape(3, number_of_particles * 8);

    std::vector<cv::Mat> projected_points;
    for (int i = 0; i < rotation_matrix_.cols; i++)
    {  // range (r_vecs)
        cv::Mat imgpoints(number_of_particles * 8, 2, CV_32FC1, cv::Scalar(0));
        cv::projectPoints(proposed_new_cube_pts_w,
                          rotation_matrix_.col(i),
                          translation_matrix_.col(i),
                          camera_matrix_,
                          distortion_coeffs_,
                          imgpoints);
        // reshape imagepoints here
        cv::Mat imgpoints_reshaped(
            number_of_particles, 8, CV_32FC2, cv::Scalar(0, 0));
        for (int j = 0; j < imgpoints_reshaped.rows; j++)
        {
            for (int k = 0; k < imgpoints_reshaped.cols; k++)
            {
                imgpoints_reshaped.at<cv::Vec2f>(j, k) =
                    cv::Vec2f(imgpoints.at<float>((j * 8) + k, 0),
                              imgpoints.at<float>((j * 8) + k, 1));
            }
        }
        projected_points.push_back(imgpoints_reshaped);
    }

    // Error matrix initialisation
    cv::Mat error;
    int use_old_method = 0;
    if (use_old_method = 1)
    {
        error = cv::Mat(number_of_particles, 1, CV_32FC1, cv::Scalar(0));
    }
    else
    {
        // TODO: _get_face_normals_cost and _cost_of_out_of_bounds_projection
        float face_normals_scaling_factor = 500;
        error = _get_face_normals_cost(pose);
        error = error + _cost_of_out_of_bounds_projection(projected_points);
        //
    }

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < rotation_matrix_.cols; i++)
    {  // range (r_vecs)
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
    finish = std::chrono::high_resolution_clock::now();
    // std::cout << "CEM projecting the points and line error calc took "
    //          << std::chrono::duration_cast<std::chrono::milliseconds>(finish
    //          -
    //                                                                   start)
    //                 .count()
    //          << " milliseconds\n";
    return error;
}

void Pose::initialise_pos_cams_w_frame()
{
    for (int i = 0; i < 3; i++)
    {
        cv::Mat pos_cam =
            getPoseMatrix(cv::Point3f(rotation_matrix_.col(i)),
                          cv::Point3f(translation_matrix_.col(i)));
        pos_cam = pos_cam.inv();
        pos_cams_w_frame_.push_back(pos_cam);
    }
}

void Pose::cross_entropy_method()
{
    int max_iterations = 50;
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

    initialise_pos_cams_w_frame();

    for (int i = 0; i < max_iterations && best_cost_ > eps; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
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
                                     number_of_particles * 10,
                                     3,
                                     "position");

            sample_o = random_normal(orientation_.mean,
                                     power(orientation_.variance, 0.5),
                                     number_of_particles * 10,
                                     3,
                                     "orientation");
        }
        auto finish = std::chrono::high_resolution_clock::now();
        // std::cout << "CEM sampling part took "
        //          << std::chrono::duration_cast<std::chrono::milliseconds>(
        //                 finish - start)
        //                 .count()
        //          << " milliseconds\n";
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

cv::Point3f Pose::mean(std::vector<cv::Point3f> points)
{
    cv::Point3f p(0., 0., 0.);
    for (auto &it : points)
    {
        p += it;
    }
    float N = 1.0 / points.size();
    p = p * N;
    return p;
}

cv::Point3f Pose::var(std::vector<cv::Point3f> points)
{
    cv::Point3f m = mean(points);
    cv::Point3f p(0., 0., 0.);
    for (auto &it : points)
    {
        p += power((it - m), 2);
    }
    float N = 1.0 / (points.size());
    p = p * N;
    return p;
}

void Pose::find_pose()
{
    auto start = std::chrono::high_resolution_clock::now();
    cross_entropy_method();  // calculates mean_position and mean_orientation
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "CEM took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(finish -
                                                                       start)
                     .count()
              << " milliseconds\n";

    if (best_cost_ > 50)
    {
        initialisation_phase_ = true;
        cross_entropy_method();
        if (best_cost_ > 50)
        {
            initialisation_phase_ = true;
        }
    }

    cv::Mat pose = getPoseMatrix(orientation_.mean, position_.mean);
    //    cv::Mat pose = getPoseMatrix(cv::Point3f(0, 0, 0),
    //    cv::Point3f(-0.0216118, -0.12, 0));

    cv::Mat proposed_new_cube_pts_w = pose * reference_center_Point_3d_.t();
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

    for (int i = 0; i < rotation_matrix_.cols; i++)
    {  // range (r_vecs)
        std::vector<cv::Point2f> imgpoints;
        cv::projectPoints(_new_pts,
                          rotation_matrix_.col(i),
                          translation_matrix_.col(i),
                          camera_matrix_,
                          distortion_coeffs_,
                          imgpoints);
        projected_points_.push_back(
            imgpoints);  // ? projected_points.append(imgpoints[:, 0, :])
    }
}

}  // namespace trifinger_object_tracking
