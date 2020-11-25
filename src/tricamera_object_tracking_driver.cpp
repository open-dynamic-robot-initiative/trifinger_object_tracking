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
               trifinger_cameras::PylonDriver(device_id_3, downsample_images)},
      cube_detector_(
          trifinger_object_tracking::create_trifingerpro_cube_detector())
{
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

    Pose cube_pose = cube_detector_.detect_cube_single_thread(images_bgr);

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

    constexpr float FILTER_CONFIDENCE_THRESHOLD = 0.75;
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
