/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

namespace trifinger_object_tracking
{
/**
 * @brief Different color segmentation models that are supported.
 *
 * Names correspond to the objects, see the documentation of supported
 * objects for more information.
 */
enum class ColorModel
{
    CUBE_V1,
    CUBE_V2,
    //! Cuboid v2 is the same material as cube v2 but this color model is
    //! fine-tuned with images of the cuboid.
    CUBOID_V2,
};
}  // namespace trifinger_object_tracking
