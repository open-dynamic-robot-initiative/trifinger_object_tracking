/**
 * @file
 * @brief Wrapper on the Pylon Driver to synchronise three pylon dependent
 * cameras.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#include <trifinger_object_tracking/tricamera_object_tracking_driver.hpp>

#include <cmath>
#include <cstdlib>
#include <thread>

#include <fmt/format.h>

#include <trifinger_cameras/parse_yml.h>

namespace trifinger_object_tracking
{
TriCameraObjectTrackerDriver::TriCameraObjectTrackerDriver(
    BaseCuboidModel::ConstPtr cube_model,
    bool downsample_images,
    std::unique_ptr<trifinger_cameras::TriCameraDriver> camera_driver,
    std::unique_ptr<TriCameraFrontend> camera_frontend)
    : downsample_images_(downsample_images),
      cube_detector_(
          trifinger_object_tracking::create_trifingerpro_cube_detector(
              cube_model, downsample_images)),
      camera_driver_(std::move(camera_driver)),
      camera_frontend_(std::move(camera_frontend))
{
    if (const char* strval =
            std::getenv("TFOT_CAMERA_FRONTEND_RATE_MULTIPLIER"))
    {
        camera_frontend_rate_multiplier_ = std::stoi(strval);
    }
    init_fope();
}

TriCameraObjectTrackerDriver::TriCameraObjectTrackerDriver(
    const std::string& device_id_1,
    const std::string& device_id_2,
    const std::string& device_id_3,
    BaseCuboidModel::ConstPtr cube_model,
    bool downsample_images,
    trifinger_cameras::Settings settings)

    : TriCameraObjectTrackerDriver(
          cube_model,
          downsample_images,
          std::make_unique<trifinger_cameras::TriCameraDriver>(
              device_id_1,
              device_id_2,
              device_id_3,
              false,  // downsample_images not supported anymore
              settings),
          nullptr)
{
}

TriCameraObjectTrackerDriver::TriCameraObjectTrackerDriver(
    const std::filesystem::path& camera_calibration_file_1,
    const std::filesystem::path& camera_calibration_file_2,
    const std::filesystem::path& camera_calibration_file_3,
    BaseCuboidModel::ConstPtr cube_model,
    bool downsample_images,
    trifinger_cameras::Settings settings)
    : TriCameraObjectTrackerDriver(
          cube_model,
          downsample_images,
          std::make_unique<trifinger_cameras::TriCameraDriver>(
              camera_calibration_file_1,
              camera_calibration_file_2,
              camera_calibration_file_3,
              false,  // downsample_images not supported anymore
              settings),
          nullptr)
{
}

TriCameraObjectTrackerDriver::TriCameraObjectTrackerDriver(
    robot_interfaces::SensorData<trifinger_cameras::TriCameraObservation,
                                 trifinger_cameras::TriCameraInfo>::Ptr
        camera_data,
    BaseCuboidModel::ConstPtr cube_model,
    bool downsample_images)
    : TriCameraObjectTrackerDriver(
          cube_model,
          downsample_images,
          nullptr,
          std::make_unique<TriCameraFrontend>(camera_data))
{
}

void TriCameraObjectTrackerDriver::init_fope()
{
    if (const char* fope_config_path = std::getenv("FOPE_CONFIG"))
    {
        fope_ = std::make_unique<fope::PoseEstimator>(
            fope::PoseEstimator::create_from_config(fope_config_path));
    }
}

trifinger_cameras::TriCameraInfo TriCameraObjectTrackerDriver::get_sensor_info()
{
    if (camera_driver_)
    {
        return camera_driver_->get_sensor_info();
    }
    else
    {
        return camera_frontend_->get_sensor_info();
    }
}

trifinger_cameras::TriCameraObservation
TriCameraObjectTrackerDriver::get_base_observation()
{
    if (camera_driver_)
    {
        return camera_driver_->get_observation();
    }
    else
    {
        // Use the latest time index if falling behind.  This results in
        // inconsistent rate and thus should normally be avoided by setting the
        // desired rate slow enough.
        auto t_current = camera_frontend_->get_current_timeindex();
        if (camera_frontend_next_timeindex_ < t_current)
        {
            fmt::print(
                "WARNING: Falling behind {} steps in "
                "TriCameraObjectTrackerDriver.\n",
                t_current - camera_frontend_next_timeindex_);
            camera_frontend_next_timeindex_ = t_current;
        }

        auto obs =
            camera_frontend_->get_observation(camera_frontend_next_timeindex_);
        camera_frontend_next_timeindex_ += camera_frontend_rate_multiplier_;

        return obs;
    }
}

TriCameraObjectObservation TriCameraObjectTrackerDriver::get_observation_color()
{
    std::array<cv::Mat, N_CAMERAS> images_bgr;

    TriCameraObjectObservation observation = get_base_observation();

    for (size_t i = 0; i < N_CAMERAS; i++)
    {
        cv::cvtColor(
            observation.cameras[i].image, images_bgr[i], cv::COLOR_BayerBG2BGR);

        // apply the downsampling that was done in trifinger_cameras in the past
        if (downsample_images_)
        {
            // remove a bit of noise
            cv::medianBlur(images_bgr[i], images_bgr[i], 3);

            // resize image
            constexpr float DOWNSAMPLING_FACTOR = 0.5;
            cv::resize(images_bgr[i],
                       images_bgr[i],
                       cv::Size(),
                       DOWNSAMPLING_FACTOR,
                       DOWNSAMPLING_FACTOR,
                       cv::INTER_LINEAR);
        }
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

TriCameraObjectObservation TriCameraObjectTrackerDriver::get_observation_fope()
{
    std::vector<cv::Mat> images_bgr(N_CAMERAS);

    TriCameraObjectObservation observation = get_base_observation();

    for (size_t i = 0; i < N_CAMERAS; i++)
    {
        cv::cvtColor(observation.cameras[i].image,
                     images_bgr[i],
                     cv::COLOR_BayerRGGB2BGR_EA);
    }

    std::vector<std::optional<fope::Pose>> poses =
        fope_->estimate_poses(images_bgr);
    std::optional<fope::Pose> pose = poses.at(0);

    if (pose)
    {
        Eigen::Matrix4d matrix;
        cv::cv2eigen(pose->pose.matrix, matrix);
        observation.object_pose = ObjectPose(matrix);
    }

    // No filtering happening here at the moment
    observation.object_pose.confidence = 1.0;
    observation.filtered_object_pose = observation.object_pose;
    previous_pose_ = observation.object_pose;

    return observation;
}

TriCameraObjectObservation TriCameraObjectTrackerDriver::get_observation()
{
    if (fope_)
    {
        return get_observation_fope();
    }
    else
    {
        return get_observation_color();
    }
}

cv::Mat TriCameraObjectTrackerDriver::get_debug_image(bool fill_faces)
{
    if (fope_)
    {
        std::vector<cv::Mat> images_bgr(N_CAMERAS);
        TriCameraObjectObservation observation = get_observation();

        for (size_t i = 0; i < N_CAMERAS; i++)
        {
            cv::cvtColor(observation.cameras[i].image,
                         images_bgr[i],
                         cv::COLOR_BayerRGGB2BGR_EA);
        }

        return fope_->visualize_last_poses(images_bgr);
    }
    else
    {
        get_observation();
        return cube_detector_.create_debug_image(fill_faces);
    }
}

}  // namespace trifinger_object_tracking
