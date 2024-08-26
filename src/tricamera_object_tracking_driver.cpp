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

#include <trifinger_cameras/parse_yml.h>

namespace trifinger_object_tracking
{
// this needs to be declared here...
constexpr std::chrono::milliseconds TriCameraObjectTrackerDriver::rate;

TriCameraObjectTrackerDriver::TriCameraObjectTrackerDriver(
    const std::string& device_id_1,
    const std::string& device_id_2,
    const std::string& device_id_3,
    BaseCuboidModel::ConstPtr cube_model,
    bool downsample_images,
    trifinger_cameras::Settings settings)
    : camera_driver_(
          device_id_1, device_id_2, device_id_3, downsample_images, settings),
      cube_detector_(
          trifinger_object_tracking::create_trifingerpro_cube_detector(
              cube_model)),
      downsample_images_(downsample_images)
{
}

TriCameraObjectObservation TriCameraObjectTrackerDriver::get_observation()
{
    std::array<cv::Mat, N_CAMERAS> images_bgr;

    TriCameraObjectObservation observation = camera_driver_.get_observation();

    for (size_t i = 0; i < N_CAMERAS; i++)
    {
        cv::cvtColor(
            observation.cameras[i].image, images_bgr[i], cv::COLOR_BayerBG2BGR);
    }

    observation.object_pose =
        cube_detector_.detect_cube_single_thread(images_bgr);

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

cv::Mat TriCameraObjectTrackerDriver::get_debug_image(bool fill_faces)
{
    get_observation();
    return cube_detector_.create_debug_image(fill_faces);
}

}  // namespace trifinger_object_tracking
