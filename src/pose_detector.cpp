#include <float.h>
#include <math.h>

#include <iostream>
#include <random>
#include <thread>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/scoped_timer.hpp>

namespace trifinger_object_tracking
{
// TODO: use quaternion instead of matrix (more efficient).
cv::Mat getPoseMatrix(const cv::Vec3f &rvec, const cv::Vec3f &tvec)
{
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
    // std::memcpy(corners_at_origin_in_world_frame_.data,
    //            cube_model_.corners_at_origin_in_world_frame,
    //            corners_at_origin_in_world_frame_.total() * sizeof(float));
    std::memcpy(corners_at_origin_in_world_frame_.data,
                cube_model_.cube_corners,
                corners_at_origin_in_world_frame_.total() * sizeof(float));
    // transform from cube frame to world frame
    corners_at_origin_in_world_frame_.col(2) += CubeModel::HALF_WIDTH;

    reference_vector_normals_ = cv::Mat(6, 3, CV_32F);
    std::memcpy(reference_vector_normals_.data,
                cube_model_.face_normal_vectors,
                reference_vector_normals_.total() * sizeof(float));
    reference_vector_normals_ = reference_vector_normals_.t();

    // Setting the bounds for pose estimation
    position_.lower_bound = cv::Vec3f(-0.25, -0.25, 0);
    position_.upper_bound = cv::Vec3f(0.25, 0.25, 0.25);

    position_.mean = (position_.upper_bound + position_.lower_bound) / 2.0;

    orientation_.lower_bound = cv::Vec3f(-3.14159265, -3.14159265, -3.14159265);
    orientation_.upper_bound = cv::Vec3f(3.14159265, 3.14159265, 3.14159265);
    orientation_.mean =
        (orientation_.upper_bound + orientation_.lower_bound) / 2.0;
}

PoseDetector::MasksPixels sample_masks_pixels_proportionally(
    const PoseDetector::MasksPixels &masks_pixels,
    const unsigned int &num_samples)
{
    unsigned int num_pixels = 0;
    for (int camera_idx = 0; camera_idx < PoseDetector::N_CAMERAS; camera_idx++)
    {
        for (size_t color_idx = 0; color_idx < masks_pixels[camera_idx].size();
             color_idx++)
        {
            num_pixels += masks_pixels[camera_idx][color_idx].size();
        }
    }

    double sampling_ratio = double(num_samples) / double(num_pixels);

    std::cout << "num_pixels: " << num_pixels
              << " ; sampling_ratio: " << sampling_ratio << std::endl;

    PoseDetector::MasksPixels sampled_masks_pixels;
    for (int camera_idx = 0; camera_idx < PoseDetector::N_CAMERAS; camera_idx++)
    {
        for (size_t color_idx = 0; color_idx < masks_pixels[camera_idx].size();
             color_idx++)
        {
            unsigned int num_pixels_in_mask =
                masks_pixels[camera_idx][color_idx].size();
            unsigned int num_samples_in_mask =
                sampling_ratio * num_pixels_in_mask;

            std::vector<cv::Point> sampled_pixels(num_samples_in_mask);
            std::sample(masks_pixels[camera_idx][color_idx].begin(),
                        masks_pixels[camera_idx][color_idx].end(),
                        sampled_pixels.begin(),
                        num_samples_in_mask,
                        std::mt19937{std::random_device{}()});

            sampled_masks_pixels[camera_idx].push_back(sampled_pixels);
        }
    }
    return sampled_masks_pixels;
}

PoseDetector::MasksPixels sample_masks_pixels(
    const PoseDetector::MasksPixels &masks_pixels,
    const unsigned int &num_samples_per_mask)
{
    PoseDetector::MasksPixels sampled_masks_pixels;
    for (int camera_idx = 0; camera_idx < PoseDetector::N_CAMERAS; camera_idx++)
    {
        for (size_t color_idx = 0; color_idx < masks_pixels[camera_idx].size();
             color_idx++)
        {
            std::vector<cv::Point> sampled_pixels(num_samples_per_mask);
            std::sample(masks_pixels[camera_idx][color_idx].begin(),
                        masks_pixels[camera_idx][color_idx].end(),
                        sampled_pixels.begin(),
                        num_samples_per_mask,
                        std::mt19937{std::random_device{}()});

            sampled_masks_pixels[camera_idx].push_back(sampled_pixels);
        }
    }

    return sampled_masks_pixels;
}

float PoseDetector::cost_function(
    const cv::Vec3f &position,
    const cv::Vec3f &orientation,
    const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
    const MasksPixels &masks_pixels,
    const float &distance_cost_scaling,
    const float &invisibility_cost_scaling,
    int *num_misclassified_pixels)
{
    float cost = 0.;
    *num_misclassified_pixels = 0;

    cv::Affine3f cube_pose_world = cv::Affine3f(orientation, position);

    cv::Mat cube_corners_world = (cv::Mat(cube_pose_world.matrix) *
                                  corners_at_origin_in_world_frame_.t());

    std::vector<cv::Point3f> cube_corners_world_vec;
    for (int j = 0; j < 8; j++)
    {
        cv::Vec3f cube_corner(cube_corners_world.col(j).rowRange(0, 3));
        // std::cout << "  corner: " << cube_corner << std::endl;
        cube_corners_world_vec.push_back(cube_corner);
    }

    for (int camera_idx = 0; camera_idx < N_CAMERAS; camera_idx++)
    {
        std::vector<cv::Point2f> imgpoints;
        cv::projectPoints(cube_corners_world_vec,
                          camera_orientations_[camera_idx],
                          camera_translations_[camera_idx],
                          camera_matrices_[camera_idx],
                          distortion_coeffs_[camera_idx],
                          imgpoints);

        cv::Mat face_normals;
        cv::Mat cube_corners;
        compute_face_normals_and_corners(
            camera_idx, cube_pose_world, &face_normals, &cube_corners);

        for (size_t color_idx = 0;
             color_idx < dominant_colors[camera_idx].size();
             color_idx++)
        {
            FaceColor color = dominant_colors[camera_idx][color_idx];

            bool face_is_visible;
            float face_normal_dot_camera_direction;
            compute_color_visibility(color,
                                     face_normals,
                                     cube_corners,
                                     &face_is_visible,
                                     &face_normal_dot_camera_direction);

            int num_misclassified_pixels_in_segment = 0;

            float distance_cost = 0;
            {
                auto corner_indices =
                    cube_model_.get_face_corner_indices(color);

                std::vector<cv::Point> corners = {imgpoints[corner_indices[0]],
                                                  imgpoints[corner_indices[1]],
                                                  imgpoints[corner_indices[2]],
                                                  imgpoints[corner_indices[3]]};

                int counter = 0;
                for (const cv::Point &pixel :
                     masks_pixels[camera_idx][color_idx])
                {
                    double dist = cv::pointPolygonTest(corners, pixel, true);

                    // negative distance means the point is outside
                    if (dist < 0)
                    {
                        num_misclassified_pixels_in_segment++;
                        distance_cost += pow(-dist, 0.5);
                    }
                }
                distance_cost *= distance_cost_scaling;
                // std::cout << "cost (visible): " << cost << std::endl;
            }

            float invisibility_cost = 0.;
            {
                // if the face of the current color is not pointing towards
                // the camera, penalize it with a cost base on the dot
                // product of the face normal and the camera-to-face vector.

                if (!face_is_visible)
                {
                    int num_pixels = masks_pixels[camera_idx][color_idx].size();

                    invisibility_cost =
                        face_normal_dot_camera_direction * num_pixels;

                    num_misclassified_pixels_in_segment = num_pixels;
                }

                invisibility_cost *= invisibility_cost_scaling;
            }

            cost += distance_cost;
            cost += invisibility_cost;
            *num_misclassified_pixels += num_misclassified_pixels_in_segment;
        }
    }

    return cost;
}

void pose2position_and_orientation(const arma::vec &pose,
                                   cv::Vec3f *position,
                                   cv::Vec3f *orientation)
{
    for (int i = 0; i < 3; i++)
    {
        (*position)(i) = pose(i);
        (*orientation)(i) = pose(3 + i);
    }
}

arma::vec position_and_orientation2pose(const cv::Vec3f &position,
                                        const cv::Vec3f &orientation)
{
    arma::vec pose(6);
    for (int i = 0; i < 3; i++)
    {
        pose(i) = position(i);
        pose(3 + i) = orientation(i);
    }

    return pose;
}

void PoseDetector::optimize_using_optim(
    const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
    const std::array<std::vector<cv::Mat>, N_CAMERAS> &masks)
{
    ScopedTimer timer("PoseDetector/optim");

    std::array<std::vector<std::vector<cv::Point>>, N_CAMERAS> masks_pixels;
    for (int camera_idx = 0; camera_idx < N_CAMERAS; camera_idx++)
    {
        for (const cv::Mat &mask : masks[camera_idx])
        {
            std::vector<cv::Point> pixels;
            cv::findNonZero(mask, pixels);
            masks_pixels[camera_idx].push_back(pixels);
        }
    }
    // downsample mask for computational efficiency
    // todo: which is the right way of sampling?
    // unsigned int num_pixels_per_mask = 15;
    // MasksPixels sampled_masks_pixels =
    //     sample_masks_pixels(masks_pixels, num_pixels_per_mask);
    unsigned int num_samples = 150;
    MasksPixels sampled_masks_pixels =
        sample_masks_pixels_proportionally(masks_pixels, num_samples);

    // todo: what is the best value here?
    float distance_cost_scaling = 5 * 1e-2;
    float invisibility_cost_scaling = 1.0;

    optim::algo_settings_t settings;
    settings.de_settings.n_gen = 50;
    settings.de_settings.n_pop = 40;
    settings.de_settings.n_pop_best = 1;
    settings.de_settings.mutation_method = 2;
    settings.print_level = 0;

    settings.vals_bound = true;
    settings.lower_bounds = {-0.35, -0.35, -0.1, -1e4, -1e4, -1e4};
    settings.upper_bounds = {0.35, 0.35, 0.35, 1e4, 1e4, 1e4};
    settings.de_settings.initial_lb = {-0.2, -0.2, 0, -1, -1, -1};
    settings.de_settings.initial_ub = {0.2, 0.2, 0.2, 1, 1, 1};

    arma::vec pose = {0., 0., 0.1250, 0., 0., 0.};

    // std::cout << "initial_pose: " << pose.t() << std::endl;
    // std::cout << "settings.lower_bounds " << settings.lower_bounds.t()
    //           << std::endl;
    // std::cout << "settings.upper_bounds " << settings.upper_bounds.t()
    //           << std::endl;
    // std::cout << "settings.de_settings.initial_lb "
    //           << settings.de_settings.initial_lb.t() << std::endl;
    // std::cout << "settings.de_settings.initial_ub "
    //           << settings.de_settings.initial_ub.t() << std::endl;

    bool success = optim::de(
        pose,
        [this,
         &dominant_colors,
         &sampled_masks_pixels,
         &distance_cost_scaling,
         &invisibility_cost_scaling](const arma::vec &pose,
                                     arma::vec *grad_out,
                                     void *opt_data) -> double {
            cv::Vec3f position;
            cv::Vec3f orientation;
            pose2position_and_orientation(pose, &position, &orientation);
            int num_misclassified_pixels;

            float cost = this->cost_function(position,
                                             orientation,
                                             dominant_colors,
                                             sampled_masks_pixels,
                                             distance_cost_scaling,
                                             invisibility_cost_scaling,
                                             &num_misclassified_pixels);

            return cost;
        },
        nullptr,
        settings);

    pose2position_and_orientation(pose, &position_.mean, &orientation_.mean);

    int num_misclassified_pixels;
    cost_function(position_.mean,
                  orientation_.mean,
                  dominant_colors,
                  sampled_masks_pixels,
                  distance_cost_scaling,
                  invisibility_cost_scaling,
                  &num_misclassified_pixels);

    info_ =
        "num_misclassified_pixels: " + std::to_string(num_misclassified_pixels);
}

Pose PoseDetector::find_pose(
    const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
    const std::array<std::vector<cv::Mat>, N_CAMERAS> &masks)
{
    ScopedTimer timer("PoseDetector/find_pose");

    // calculates mean_position and mean_orientation
    optimize_using_optim(dominant_colors, masks);

    return Pose(position_.mean, orientation_.mean);
}

std::vector<std::vector<cv::Point2f>> PoseDetector::get_projected_points() const
{
    std::vector<std::vector<cv::Point2f>> projected_points;

    cv::Mat pose = getPoseMatrix(orientation_.mean, position_.mean);

    // FIXME store differently to avoid transposing here
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

bool PoseDetector::is_face_visible(FaceColor color,
                                   unsigned int camera_idx,
                                   const cv::Affine3f &cube_pose_world,
                                   float *out_dot_product) const
{
    cv::Mat face_normals;
    cv::Mat cube_corners;

    compute_face_normals_and_corners(
        camera_idx, cube_pose_world, &face_normals, &cube_corners);

    bool is_visible;
    float face_normal_dot_camera_direction;
    compute_color_visibility(color,
                             face_normals,
                             cube_corners,
                             &is_visible,
                             &face_normal_dot_camera_direction);

    if (out_dot_product != nullptr)
    {
        *out_dot_product = face_normal_dot_camera_direction;
    }

    return is_visible;
}

bool PoseDetector::compute_face_normals_and_corners(
    const unsigned int camera_idx,
    const cv::Affine3f &cube_pose_world,
    cv::Mat *normals,
    cv::Mat *corners) const
{
    cv::Affine3f camera_pose(camera_orientations_[camera_idx],
                             camera_translations_[camera_idx]);

    // cube pose in camera frame
    cv::Affine3f cube_pose_camera = camera_pose * cube_pose_world;

    // TODO only transform the normal vector and corner that are actually used
    // rotate face normals (3x6) according to given cube pose
    *normals = cv::Mat(cube_pose_camera.rotation()) * reference_vector_normals_;

    // transform all cube corners according to the cube pose (4x8)
    *corners = cv::Mat(cube_pose_camera.matrix) *
               corners_at_origin_in_world_frame_.t();
}

bool PoseDetector::compute_color_visibility(
    const FaceColor &color,
    const cv::Mat &face_normals,
    const cv::Mat &cube_corners,
    bool *is_visible,
    float *face_normal_dot_camera_direction) const
{
    // get the normal vector of that face
    // <<<<<<< HEAD
    int normal_idx = cube_model_.map_color_to_face[color];
    // cv::Vec3f face_normal = face_normal_vectors.col(normal_idx);
    // =======
    // int normal_idx = cube_model_.map_color_to_normal_index[color];
    cv::Vec3f face_normal = face_normals.col(normal_idx);
    // >>>>>>> manuel/optimizing_cem

    auto corner_indices = cube_model_.get_face_corner_indices(color);

    // get an arbitrary corner of that face
    unsigned int corner_idx = corner_indices[0];
    cv::Vec3f corner = cube_corners.col(corner_idx).rowRange(0, 3);

    // if the angle between the face normal and the camera-to-corner
    // vector is greater than 90 deg, the face is visible
    *face_normal_dot_camera_direction =
        face_normal.dot(corner) / cv::norm(face_normal) / cv::norm(corner);

    // dot_prod < 0 ==> angle > 90 deg
    *is_visible = (*face_normal_dot_camera_direction < 0);
}

std::vector<std::pair<FaceColor, std::array<unsigned int, 4>>>
PoseDetector::get_visible_faces(unsigned int camera_idx,
                                const cv::Affine3f &cube_pose_world) const
{
    std::vector<std::pair<FaceColor, std::array<unsigned int, 4>>> result;

    // check for each color if the face is visible
    for (FaceColor color : cube_model_.get_colors())
    {
        if (is_face_visible(color, camera_idx, cube_pose_world))
        {
            auto corner_indices = cube_model_.get_face_corner_indices(color);
            result.push_back(std::make_pair(color, corner_indices));
        }
    }

    return result;
}

std::vector<std::pair<FaceColor, std::array<unsigned int, 4>>>
PoseDetector::get_visible_faces(unsigned int camera_idx) const
{
    cv::Affine3f cube_pose_world(orientation_.mean, position_.mean);

    return get_visible_faces(camera_idx, cube_pose_world);
}

}  // namespace trifinger_object_tracking
