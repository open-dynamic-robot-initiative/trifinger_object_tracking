/**
 * @file
 * @brief Create bindings for three pylon dependent camera sensors
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#include <pybind11/eigen.h>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <trifinger_object_tracking/pybullet_tricamera_object_tracker_driver.hpp>
#ifdef Pylon_FOUND
#include <trifinger_object_tracking/tricamera_object_tracking_driver.hpp>
#endif

#include <robot_interfaces/sensors/pybind_sensors.hpp>
#include <robot_interfaces/sensors/sensor_driver.hpp>

using namespace robot_interfaces;
using namespace trifinger_object_tracking;

PYBIND11_MODULE(py_tricamera_types, m)
{
    // import for Python bindings of ObjectPose
    pybind11::module::import("trifinger_object_tracking.py_object_tracker");

    create_sensor_bindings<TriCameraObjectObservation>(m);

    pybind11::class_<TriCameraObjectObservation>(
        m,
        "TriCameraObjectObservation",
        "Observation from the three cameras, including the estimated object "
        "pose provided by the integrated object tracker.")
        .def(pybind11::init<>())
        .def_readwrite(
            "cameras",
            &TriCameraObjectObservation::cameras,
            "List[CameraObservation]: List of observations from cameras "
            "'camera60', 'camera180' and 'camera300' (in this order)")
        .def_readwrite("object_pose",
                       &TriCameraObjectObservation::object_pose,
                       "ObjectPose: Estimated object pose.");

#ifdef Pylon_FOUND
    pybind11::class_<TriCameraObjectTrackerDriver,
                     std::shared_ptr<TriCameraObjectTrackerDriver>,
                     SensorDriver<TriCameraObjectObservation>>(
        m, "TriCameraObjectTrackerDriver")
        .def(pybind11::init<const std::string&,
                            const std::string&,
                            const std::string&,
                            bool>(),
             pybind11::arg("camera1"),
             pybind11::arg("camera2"),
             pybind11::arg("camera3"),
             pybind11::arg("downsample_images") = true)
        .def("get_observation", &TriCameraObjectTrackerDriver::get_observation);
#endif

    pybind11::class_<PyBulletTriCameraObjectTrackerDriver,
                     std::shared_ptr<PyBulletTriCameraObjectTrackerDriver>,
                     SensorDriver<TriCameraObjectObservation>>(
        m, "PyBulletTriCameraObjectTrackerDriver")
        .def(pybind11::init<pybind11::object,
                            robot_interfaces::TriFingerTypes::BaseDataPtr,
                            bool>())
        .def("get_observation",
             &PyBulletTriCameraObjectTrackerDriver::get_observation);
}
