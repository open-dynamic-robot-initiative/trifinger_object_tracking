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
          trifinger_object_tracking::TriCameraObjectObservation>
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
        bool render_images = true)
        : tracking_object_(tracking_object),
          robot_data_(robot_data),
          render_images_(render_images),
          last_update_robot_time_index_(0)
    {
        // initialize Python interpreter if not already done
        if (!Py_IsInitialized())
        {
            pybind11::initialize_interpreter();
        }

        pybind11::gil_scoped_acquire acquire;

        if (render_images)
        {
            // some imports that are needed later for converting images (numpy
            // array
            // -> cv::Mat)
            numpy_ = pybind11::module::import("numpy");
            pybind11::module camera_types =
                pybind11::module::import("trifinger_cameras.py_camera_types");
            cvMat_ = camera_types.attr("cvMat");

            // TriFingerCameras gives access to the cameras in simulation
            pybind11::module mod_camera =
                pybind11::module::import("trifinger_simulation.camera");
            cameras_ = mod_camera.attr("TriFingerCameras")();
        }
    }

    //! @brief Get the latest observation.
    trifinger_object_tracking::TriCameraObjectObservation get_observation();

private:
    //! @brief Python object to access cameras in pyBullet.
    pybind11::object cameras_;

    //! @brief Python object of the tracked object.
    pybind11::object tracking_object_;

    robot_interfaces::TriFingerTypes::BaseDataPtr robot_data_;

    //! @brief If false, no actual images are rendered.
    bool render_images_;

    //! @brief The numpy module.
    pybind11::module numpy_;
    //! @brief Python class that wraps cv::Mat (used for conversion).
    pybind11::object cvMat_;

    time_series::Index last_update_robot_time_index_;
};

}  // namespace trifinger_object_tracking

