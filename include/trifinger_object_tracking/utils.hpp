#pragma once

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

#include <trifinger_cameras/camera_parameters.hpp>


namespace trifinger_object_tracking
{

/**
 * @brief Load camera calibration parameters from YAML files.
 *
 * @param parameter_files List of YAML files.  Expected order is camera60,
 *     camera180, camera300.
 *
 * @return Camera calibration parameters.
 * @throw std::runtime_error if loading of the parameters fails.
 */
std::array<trifinger_cameras::CameraParameters, 3> load_camera_parameters(
    std::array<std::string, 3> parameter_files);

}  // namespace trifinger_object_tracking
