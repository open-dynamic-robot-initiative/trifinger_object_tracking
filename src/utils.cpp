/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 * @license BSD 3-clause
 */
#include <trifinger_object_tracking/utils.hpp>

#include <trifinger_cameras/parse_yml.h>

namespace trifinger_object_tracking
{
std::array<trifinger_cameras::CameraParameters, 3> load_camera_parameters(
    std::array<std::string, 3> parameter_files)
{
    static const std::string camera_names[3] = {
        "camera60", "camera180", "camera300"};

    std::array<trifinger_cameras::CameraParameters, 3> camera_params;

    for (size_t i = 0; i < 3; i++)
    {
        bool success;
        std::string camera_name;

        success = trifinger_cameras::readCalibrationYml(
            parameter_files[i], camera_name, camera_params[i]);
        if (!success)
        {
            throw std::runtime_error("Failed to load parameters of " +
                                     camera_names[i]);
        }
        if (camera_name != camera_names[i])
        {
            throw std::runtime_error(
                "Camera name in calibration file does not match with " +
                camera_names[i]);
        }
    }

    return camera_params;
}
}  // namespace trifinger_object_tracking
