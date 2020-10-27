#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <armadillo>
#include <boost/algorithm/string.hpp>
#include <trifinger_object_tracking/utils.hpp>
#include <trifinger_cameras/parse_yml.h>


namespace fs = boost::filesystem;

namespace trifinger_object_tracking
{
std::array<trifinger_cameras::CameraParameters, 3> load_camera_parameters(
    std::array<std::string, 3> parameter_files)
{
    std::array<trifinger_cameras::CameraParameters, 3> camera_params;
    std::string camera_name;
    bool success;

    success = trifinger_cameras::readCalibrationYml(
        parameter_files[0], camera_name, camera_params[0]);
    if (!success || camera_name != "camera60")
    {
        throw std::runtime_error("Failed to load parameters of camera60 ");
    }

    success = trifinger_cameras::readCalibrationYml(
        parameter_files[1], camera_name, camera_params[1]);
    if (!success || camera_name != "camera180")
    {
        throw std::runtime_error("Failed to load parameters of camera180");
    }

    success = trifinger_cameras::readCalibrationYml(
        parameter_files[2], camera_name, camera_params[2]);
    if (!success || camera_name != "camera300")
    {
        throw std::runtime_error("Failed to load parameters of camera300");
    }

    return camera_params;
}
}  // namespace trifinger_object_tracking
