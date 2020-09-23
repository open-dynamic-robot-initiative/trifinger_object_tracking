/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/cv_sub_images.hpp>
#include <trifinger_object_tracking/line_detector.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/scoped_timer.hpp>

#define VISUALIZE

namespace trifinger_object_tracking
{
/**
 * @brief Detect coloured cube in images from a three-camera setup.
 */
class CubeDetector
{
public:
    static constexpr unsigned int N_CAMERAS = 3;

    /**
     * @param segmentation_model_dir Directory containing the segmentation model
     * @param camera_params Calibration parameters of the cameras.
     */
    CubeDetector(const std::string &segmentation_model_dir,
                 const std::array<trifinger_cameras::CameraParameters,
                                  N_CAMERAS> &camera_params);

    /**
     * @brief Detect cube in the given images
     *
     * @param images Images from cameras camera60, camera180, camera300.
     *
     * @return Pose of the cube.
     */
    Pose detect_cube(const std::array<cv::Mat, N_CAMERAS> &images);

    /**
     * @brief Create debug image for the last call of detect_cube.
     *
     * @return Aggregate image showing different stages of the cube detection.
     */
    cv::Mat create_debug_image() const;

private:
    CubeModel cube_model_;
    std::array<LineDetector, N_CAMERAS> line_detectors_;
    PoseDetector pose_detector_;
};
}  // namespace trifinger_object_tracking

