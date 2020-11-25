/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/cube_model.hpp>

namespace trifinger_object_tracking
{
struct Pose
{
    // TODO use non-cv types?

    cv::Vec3f translation;
    //! rotation vector
    cv::Vec3f rotation;

    float confidence = 0.0;

    Pose(const cv::Vec3f &translation,
         const cv::Vec3f &rotation,
         float confidence = 0.0)
        : translation(translation), rotation(rotation), confidence(confidence)
    {
    }
};

}  // namespace trifinger_object_tracking
