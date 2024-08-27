/**
 * @file
 * @brief TriCameraObjectTrackerDriver for simulation (using rendered images).
 * cameras.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 */
#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>

#include <robot_interfaces/finger_types.hpp>
#include <robot_interfaces/sensors/sensor_driver.hpp>
#include <trifinger_cameras/camera_parameters.hpp>
#include <trifinger_cameras/pybullet_tricamera_driver.hpp>

#include <trifinger_object_tracking/tricamera_object_observation.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Driver to get rendered camera images and object pose from pyBullet.
 *
 * This is a simulation-based replacement for TriCameraObjectTrackerDriver.
 */
class PyBulletTriCameraObjectTrackerDriver
    : public robot_interfaces::SensorDriver<
          trifinger_object_tracking::TriCameraObjectObservation,
          trifinger_cameras::TriCameraInfo>
{
public:
    // For some reason the constructor needs to be in the header file to avoid a
    // linker error with pybind11::object...

    /**
     * @param tracking_object Python object of the tracked object.  Needs to
     *     have a method `get_state()` which returns a tuple of object position
     *     (x, y, z) and object orientation quaternion (x, y, z, w).
     * @param render_images Set to false to disable image rendering.  Images in
     *     the observations will be uninitialized.  Use this if you only need
     *     the object pose and are not interested in the actual images.
     *     Rendering is pretty slow, so by disabling it, the simulation may run
     *     much faster.
     */
    PyBulletTriCameraObjectTrackerDriver(
        pybind11::object tracking_object,
        robot_interfaces::TriFingerTypes::BaseDataPtr robot_data,
        bool render_images = true,
        trifinger_cameras::Settings settings = trifinger_cameras::Settings());

    /**
     * @brief Get the camera parameters.
     *
     * @verbatim embed:rst:leading-asterisk
     * Internally, this calls
     * :cpp:func:`trifinger_cameras::PyBulletTriCameraDriver::get_sensor_info`,
     * so see there fore more information.
     * @endverbatim
     */
    trifinger_cameras::TriCameraInfo get_sensor_info() override;

    //! @brief Get the latest observation.
    trifinger_object_tracking::TriCameraObjectObservation get_observation()
        override;

private:
    //! @brief PyBullet driver instance for rendering images.
    trifinger_cameras::PyBulletTriCameraDriver camera_driver_;

    //! @brief Python object of the tracked object.
    pybind11::object tracking_object_;
};

}  // namespace trifinger_object_tracking
