/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <chrono>
#include <filesystem>
#include <memory>

#include <robot_interfaces/sensors/sensor_data.hpp>
#include <robot_interfaces/sensors/sensor_driver.hpp>
#include <robot_interfaces/sensors/sensor_frontend.hpp>
#include <trifinger_cameras/camera_parameters.hpp>
#include <trifinger_cameras/pylon_driver.hpp>
#include <trifinger_cameras/settings.hpp>
#include <trifinger_cameras/tricamera_driver.hpp>

#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/tricamera_object_observation.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Driver to create three instances of the PylonDriver
 * and get observations from them.
 */
class TriCameraObjectTrackerDriver
    : public robot_interfaces::SensorDriver<TriCameraObjectObservation,
                                            trifinger_cameras::TriCameraInfo>
{
public:
    //! @brief Rate at which images are acquired.
    static constexpr std::chrono::milliseconds rate =
        std::chrono::milliseconds(100);

    static constexpr unsigned int N_CAMERAS = 3;

    /**
     * @param device_id_1 device user id of first camera
     * @param device_id_2 likewise, the 2nd's
     * @param device_id_3 and the 3rd's
     * @param cube_model The model that is used for detecting the cube.
     * @param downsample_images If set to true (default), images are
     *     downsampled to half their original size for object tracking.
     * @param settings Settings for the cameras.
     */
    TriCameraObjectTrackerDriver(
        const std::string& device_id_1,
        const std::string& device_id_2,
        const std::string& device_id_3,
        BaseCuboidModel::ConstPtr cube_model,
        bool downsample_images = true,
        trifinger_cameras::Settings settings = trifinger_cameras::Settings());

    /**
     * @param camera_calibration_file_1 Calibration file of first camera
     * @param camera_calibration_file_2 likewise, the 2nd's
     * @param camera_calibration_file_3 and the 3rd's
     * @param cube_model The model that is used for detecting the cube.
     * @param downsample_images If set to true (default), images are
     *     downsampled to half their original size for object tracking.
     * @param settings Settings for the cameras.
     */
    TriCameraObjectTrackerDriver(
        const std::filesystem::path& camera_calibration_file_1,
        const std::filesystem::path& camera_calibration_file_2,
        const std::filesystem::path& camera_calibration_file_3,
        BaseCuboidModel::ConstPtr cube_model,
        bool downsample_images = true,
        trifinger_cameras::Settings settings = trifinger_cameras::Settings());

    TriCameraObjectTrackerDriver(
        robot_interfaces::SensorData<trifinger_cameras::TriCameraObservation,
                                     trifinger_cameras::TriCameraInfo>::Ptr
            camera_data,
        BaseCuboidModel::ConstPtr cube_model,
        bool downsample_images = true);

    /**
     * @brief Get the camera parameters.
     *
     * @rst
     * Internally, this calls
     * :cpp:func:`trifinger_cameras::TriCameraDriver::get_sensor_info`, so see
     * there fore more information.
     * @endrst
     */
    trifinger_cameras::TriCameraInfo get_sensor_info() override;

    /**
     * @brief Get the latest observation from the three cameras.
     * @return TricameraObservation
     */
    TriCameraObjectObservation get_observation() override;

    /**
     * @brief Fetch an observation from the cameras and create a debug image.
     *
     * The debug image shows various visualisations of the object detection, see
     * @ref CubeDetector::create_debug_image.
     *
     * @see CubeDetector::create_debug_image
     * @param fill_faces
     *
     * @return Debug image showing the result of the object detection.
     */
    cv::Mat get_debug_image(bool fill_faces = false);

private:
    typedef robot_interfaces::SensorFrontend<
        trifinger_cameras::TriCameraObservation,
        trifinger_cameras::TriCameraInfo>
        TriCameraFrontend;

    bool downsample_images_ = true;
    trifinger_object_tracking::CubeDetector cube_detector_;

    std::unique_ptr<trifinger_cameras::TriCameraDriver> camera_driver_;
    std::unique_ptr<TriCameraFrontend> camera_frontend_;
    TriCameraFrontend::TimeIndex camera_frontend_next_timeindex_ = 0;

    ObjectPose previous_pose_;

    trifinger_cameras::TriCameraObservation get_base_observation();
};

}  // namespace trifinger_object_tracking
