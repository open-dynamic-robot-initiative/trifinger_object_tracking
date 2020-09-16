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
struct ColorBounds
{
    int lower[3];
    int upper[3];
};

struct Line
{
    float a;
    float b;

    Line() : a(0.0), b(0.0)
    {
    }

    Line(float a, float b) : a(a), b(b)
    {
    }
};

struct Pose
{
    // TODO use non-cv types?

    cv::Point3f translation;
    //! rotation vector
    cv::Point3f rotation;

    Pose(const cv::Point3f &translation, const cv::Point3f &rotation)
        : translation(translation), rotation(rotation)
    {
    }
};

typedef std::pair<FaceColor, FaceColor> ColorPair;

}  // namespace trifinger_object_tracking
