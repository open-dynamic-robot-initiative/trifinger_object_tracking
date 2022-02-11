/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <array>

#include "color_model.hpp"

namespace trifinger_object_tracking
{
constexpr int XGB_NUM_CLASSES = 7;
constexpr int XGB_NUM_FEATURES = 6;

std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v1(
    std::array<float, XGB_NUM_FEATURES> &sample);

std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v2(
    std::array<float, XGB_NUM_FEATURES> &sample);

std::array<float, XGB_NUM_CLASSES> xgb_classify_cuboid_v2(
    std::array<float, XGB_NUM_FEATURES> &sample);

inline std::array<float, XGB_NUM_CLASSES> xgb_classify(
    const ColorModel color_model, std::array<float, XGB_NUM_FEATURES> &sample)
{
    switch (color_model)
    {
        case ColorModel::CUBE_V1:
            return xgb_classify_cube_v1(sample);
        case ColorModel::CUBE_V2:
            return xgb_classify_cube_v2(sample);
        case ColorModel::CUBOID_V2:
            return xgb_classify_cuboid_v2(sample);
        default:
            // this should never happen
            throw std::runtime_error("Unsupported color model");
    }
}
}  // namespace trifinger_object_tracking
