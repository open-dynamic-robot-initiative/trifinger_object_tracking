#pragma once

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

#include <trifinger_cameras/camera_parameters.hpp>


namespace trifinger_object_tracking
{
cv::Mat get_image(const std::string &path, const int idx);

std::vector<std::string> get_directories(const std::string &s);

bool comparator(const std::string &a, const std::string &b);

std::map<char, std::string> initialize();

std::map<std::string, cv::Mat> get_color_masks(
    const std::string &path, std::map<std::string, cv::Mat> dataset);

void train_gmm(std::map<std::string, cv::Mat> dataset);

bool is_zero(const cv::Mat &rc);

typedef bool (*remove_predicate)(const cv::Mat &rc);

void remove_if(const cv::Mat &mat,
               cv::Mat &res,
               remove_predicate pred,
               bool removeRows);


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
