/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <trifinger_cameras/parse_yml.h>

#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/object_pose.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/utils.hpp>

namespace trifinger_object_tracking
{
class CubeVisualizer
{
public:
    static constexpr unsigned int N_CAMERAS = 3;

    CubeVisualizer(BaseCuboidModel::ConstPtr cube_model,
                   const std::array<trifinger_cameras::CameraParameters,
                                    N_CAMERAS> &camera_params)
        : cube_model_(cube_model), pose_detector_(cube_model, camera_params)
    {
    }

    CubeVisualizer(BaseCuboidModel::ConstPtr cube_model,
                   const std::array<std::string, N_CAMERAS> &camera_calib_files)
        : CubeVisualizer(cube_model, load_camera_parameters(camera_calib_files))
    {
    }

    std::array<cv::Mat, N_CAMERAS> draw_cube(
        const std::array<cv::Mat, N_CAMERAS> &images,
        const ObjectPose &object_pose,
        bool fill_faces = false);

private:
    BaseCuboidModel::ConstPtr cube_model_;
    PoseDetector pose_detector_;
};
}  // namespace trifinger_object_tracking
