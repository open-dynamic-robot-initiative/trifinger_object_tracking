/**
 * @file
 * @brief Wrapper on the Pylon Driver to synchronise three pylon dependent
 * cameras.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 */
#include <trifinger_object_tracking/pybullet_tricamera_object_tracker_driver.hpp>

#include <chrono>
#include <thread>

#include <pybind11/eigen.h>
#include <pybind11/embed.h>

#include <pybind11_opencv/cvbind.hpp>
#include <serialization_utils/cereal_cvmat.hpp>

namespace py = pybind11;

namespace trifinger_object_tracking
{
PyBulletTriCameraObjectTrackerDriver::PyBulletTriCameraObjectTrackerDriver(
    pybind11::object tracking_object,
    robot_interfaces::TriFingerTypes::BaseDataPtr robot_data,
    bool render_images,
    trifinger_cameras::Settings settings)
    : camera_driver_(robot_data, render_images, settings),
      tracking_object_(tracking_object)
{
}

trifinger_cameras::TriCameraInfo
PyBulletTriCameraObjectTrackerDriver::get_sensor_info()
{
    return camera_driver_.get_sensor_info();
}

TriCameraObjectObservation
PyBulletTriCameraObjectTrackerDriver::get_observation()
{
    trifinger_object_tracking::TriCameraObjectObservation observation =
        camera_driver_.get_observation();

    {
        py::gil_scoped_acquire acquire;

        pybind11::tuple state = tracking_object_.attr("get_state")();
        observation.object_pose.position = state[0].cast<Eigen::Vector3d>();
        observation.object_pose.orientation = state[1].cast<Eigen::Vector4d>();
    }

    // there is no noise in the simulation
    observation.object_pose.confidence = 1.0;

    // no noise, so no actual filtering needed
    observation.filtered_object_pose = observation.object_pose;

    return observation;
}

}  // namespace trifinger_object_tracking
