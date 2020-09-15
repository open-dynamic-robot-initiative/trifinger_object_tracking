/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

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

typedef std::pair<FaceColor, FaceColor> ColorPair;

}  // namespace trifinger_object_tracking
