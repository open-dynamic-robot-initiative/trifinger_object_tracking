/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <chrono>

#include <robot_interfaces/sensors/sensor_driver.hpp>
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
     *     downsampled to half their original size.
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
     * @brief Get the camera parameters (image size and calibration
     * coefficients).
     *
     * **Important:**  The calibration coefficients are only set if the driver
     * is initialized with calibration files (see constructor).  Otherwise,
     * they will be empty.
     */
    virtual trifinger_cameras::TriCameraInfo get_sensor_info() override;

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
    trifinger_cameras::TriCameraDriver camera_driver_;
    // std::array<trifinger_cameras::PylonDriver, N_CAMERAS> cameras_;
    trifinger_object_tracking::CubeDetector cube_detector_;
    // TODO unused?
    bool downsample_images_;

    ObjectPose previous_pose_;
};

}  // namespace trifinger_object_tracking
