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
 * @brief Wrapper around ``trifinger_cameras::TriCameraDriver`` that adds object
 *        tracking.
 *
 * @rst
 * This class serves as a drop-in replacement (more or less) for
 * :cpp:class:`trifinger_cameras::TriCameraDriver`.  It extends the latter with
 * object tracking using the method provided by this package.  The resulting
 * object pose is added to the camera observation.
 *
 * There are two methods for providing the camera images to the wrapper:
 *
 * 1. Pass three camera names or calibration files to the constructor.  In this
 *    case an internal instance of
 *    :cpp:class:`trifinger_cameras::TriCameraDriver` is created and used to get
 *    the images.
 * 2. Pass a :cpp:class:`robot_interfaces::SensorData` object that is connected
 *    to a backend running the :cpp:class:`~trifinger_cameras::TriCameraDriver`.
 *
 * When method 1 is used, the driver will always run at the rate of the internal
 * :cpp:class:`~trifinger_cameras::TriCameraDriver` instance (or as fast as a
 * possible, if the pose estimation is slower than the camera rate). When method
 * 2 is used, the driver will by default run at the rate at which the sensor
 * backend provides observations but it can be stepped down by setting a
 * multiplier to the environment variable
 * ``TFOT_CAMERA_FRONTEND_RATE_MULTIPLIER``.  For example, set
 * ``TFOT_CAMERA_FRONTEND_RATE_MULTIPLIER=5`` to run at 1/5 the rate of the
 * observations provided through the given sensor data object.  This can be
 * useful to maintain a constant rate of the object tracking driver in settings
 * where the camera runs at a rate that is faster than the time needed for the
 * pose estimation.
 *
 * While method 2 is more complex to set up, it allows for more flexibility as
 * the object tracking can run at a lower rate than the camera.
 *
 * @endrst
 */
class TriCameraObjectTrackerDriver
    : public robot_interfaces::SensorDriver<TriCameraObjectObservation,
                                            trifinger_cameras::TriCameraInfo>
{
public:
    //! Number of cameras.
    static constexpr unsigned int N_CAMERAS = 3;

    /**
     * @brief Construct with internal TriCameraDriver using device IDs.
     *
     * @param device_id_1 device user id of first camera
     * @param device_id_2 likewise, the 2nd's
     * @param device_id_3 and the 3rd's
     * @param cube_model The model that is used for detecting the cube.
     * @param downsample_images If set to true (default), images are
     *     downsampled to half their original size for object tracking.
     * @param settings Settings for the cameras.  This is passed to the internal
     *     TriCameraDriver.
     */
    TriCameraObjectTrackerDriver(
        const std::string& device_id_1,
        const std::string& device_id_2,
        const std::string& device_id_3,
        BaseCuboidModel::ConstPtr cube_model,
        bool downsample_images = true,
        trifinger_cameras::Settings settings = trifinger_cameras::Settings());

    /**
     * @brief Construct with internal TriCameraDriver using calibration files.
     *
     * @param camera_calibration_file_1 Calibration file of first camera
     * @param camera_calibration_file_2 likewise, the 2nd's
     * @param camera_calibration_file_3 and the 3rd's
     * @param cube_model The model that is used for detecting the cube.
     * @param downsample_images If set to true (default), images are
     *     downsampled to half their original size for object tracking.
     * @param settings Settings for the cameras.  This is passed to the internal
     *     TriCameraDriver.
     */
    TriCameraObjectTrackerDriver(
        const std::filesystem::path& camera_calibration_file_1,
        const std::filesystem::path& camera_calibration_file_2,
        const std::filesystem::path& camera_calibration_file_3,
        BaseCuboidModel::ConstPtr cube_model,
        bool downsample_images = true,
        trifinger_cameras::Settings settings = trifinger_cameras::Settings());

    /**
     * @brief Construct with external camera data.
     *
     * Expects that a separate SensorBackend is running in parallel, which
     * provides TriCameraObservations to the given SensorData object.
     *
     * @param camera_data The sensor data object that provides the camera
     * images.
     * @param cube_model The model that is used for detecting the cube.
     * @param downsample_images If set to true (default), images are
     *     downsampled to half their original size for object tracking.
     */
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
    unsigned int camera_frontend_rate_multiplier_ =
        1;  // FIXME: make configurable

    ObjectPose previous_pose_;

    TriCameraObjectTrackerDriver(
        BaseCuboidModel::ConstPtr cube_model,
        bool downsample_images,
        std::unique_ptr<trifinger_cameras::TriCameraDriver> camera_driver,
        std::unique_ptr<TriCameraFrontend> camera_frontend);

    trifinger_cameras::TriCameraObservation get_base_observation();
};

}  // namespace trifinger_object_tracking
