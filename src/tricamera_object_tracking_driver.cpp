/**
 * @file
 * @brief Wrapper on the Pylon Driver to synchronise three pylon dependent
 * cameras.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#include <trifinger_object_tracking/tricamera_object_tracking_driver.hpp>

#include <cmath>
#include <thread>

#include <opencv2/core/eigen.hpp>

#include <trifinger_cameras/parse_yml.h>

namespace trifinger_object_tracking
{
// this needs to be declared here...
constexpr std::chrono::milliseconds TriCameraObjectTrackerDriver::rate;

TriCameraObjectTrackerDriver::TriCameraObjectTrackerDriver(
    const std::string& device_id_1,
    const std::string& device_id_2,
    const std::string& device_id_3,
    bool downsample_images)
    : last_update_time_(std::chrono::system_clock::now()),
      downsample_images_(downsample_images),
      cameras_{trifinger_cameras::PylonDriver(device_id_1, downsample_images),
               trifinger_cameras::PylonDriver(device_id_2, downsample_images),
               trifinger_cameras::PylonDriver(device_id_3, downsample_images)}
{
    std::array<trifinger_cameras::CameraParameters, 3> camera_params;
    std::string camera_name;
    bool success;
    success = trifinger_cameras::readCalibrationYml(
        "/etc/trifingerpro/camera60_cropped_and_downsampled.yml",
        camera_name,
        camera_params[0]);
    if (!success)
    {
        throw std::runtime_error(
            "Failed to load calibration parameters of camera60.");
    }
    if (camera_name != "camera60")
    {
        throw std::runtime_error(
            "Camera name in calibration file does not match with camera60.");
    }

    success = trifinger_cameras::readCalibrationYml(
        "/etc/trifingerpro/camera180_cropped_and_downsampled.yml",
        camera_name,
        camera_params[1]);
    if (!success)
    {
        throw std::runtime_error(
            "Failed to load calibration parameters of camera180.");
    }
    if (camera_name != "camera180")
    {
        throw std::runtime_error(
            "Camera name in calibration file does not match with camera180.");
    }

    success = trifinger_cameras::readCalibrationYml(
        "/etc/trifingerpro/camera300_cropped_and_downsampled.yml",
        camera_name,
        camera_params[2]);
    if (!success)
    {
        throw std::runtime_error(
            "Failed to load calibration parameters of camera300.");
    }
    if (camera_name != "camera300")
    {
        throw std::runtime_error(
            "Camera name in calibration file does not match with camera300.");
    }

    cube_detector_ = std::make_unique<trifinger_object_tracking::CubeDetector>(
        camera_params);
}

TriCameraObjectObservation TriCameraObjectTrackerDriver::get_observation()
{
    last_update_time_ += this->rate;
    std::this_thread::sleep_until(last_update_time_);

    std::array<cv::Mat, N_CAMERAS> images_bgr;
    TriCameraObjectObservation observation;

    for (size_t i = 0; i < N_CAMERAS; i++)
    {
        observation.cameras[i] = cameras_[i].get_observation();

        cv::cvtColor(
            observation.cameras[i].image, images_bgr[i], cv::COLOR_BayerBG2BGR);
    }

    Pose cube_pose = cube_detector_->detect_cube_single_thread(images_bgr);

    // convert rotation vector to quaternion
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
    //
    //     qx = ax * sin(angle/2)
    //     qy = ay * sin(angle/2)
    //     qz = az * sin(angle/2)
    //     qw = cos(angle/2)
    //
    float angle = cv::norm(cube_pose.rotation);
    cube_pose.rotation *= std::sin(angle / 2) / angle;
    cv::Vec4d quaternion(cube_pose.rotation[0],
                         cube_pose.rotation[1],
                         cube_pose.rotation[2],
                         std::cos(angle / 2));

    cv::cv2eigen(static_cast<cv::Vec3d>(cube_pose.translation),
                 observation.object_pose.position);
    cv::cv2eigen(quaternion, observation.object_pose.orientation);
    observation.object_pose.confidence = cube_pose.confidence;

    constexpr float FILTER_CONFIDENCE_THRESHOLD = 0.7;
    constexpr float FILTER_CONFIDENCE_DEGRADATION = 0.9;
    if (previous_pose_.confidence > 0 &&
        observation.object_pose.confidence < FILTER_CONFIDENCE_THRESHOLD)
    {
        // every time a pose is reused, degrade its confidence a bit
        previous_pose_.confidence *= FILTER_CONFIDENCE_DEGRADATION;
        observation.filtered_object_pose = previous_pose_;
    }
    else
    {
        observation.filtered_object_pose = observation.object_pose;
        previous_pose_ = observation.object_pose;
    }

    return observation;
}

}  // namespace trifinger_object_tracking
