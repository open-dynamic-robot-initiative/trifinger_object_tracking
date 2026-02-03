/**
 * @file
 * @brief Create bindings for three pylon dependent camera sensors
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#include <pybind11/eigen.h>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl_bind.h>

#include <pybind11_opencv/cvbind.hpp>
#include <trifinger_object_tracking/pybullet_tricamera_object_tracker_driver.hpp>
#ifdef Pylon_FOUND
#include <trifinger_object_tracking/tricamera_object_tracking_driver.hpp>
#endif

#include <robot_interfaces/sensors/pybind_sensors.hpp>
#include <robot_interfaces/sensors/sensor_driver.hpp>
#include <trifinger_object_tracking/cube_visualizer.hpp>

using namespace robot_interfaces;
using namespace trifinger_object_tracking;

PYBIND11_MODULE(py_tricamera_types, m)
{
    // import for Python bindings of CameraObservation
    pybind11::module::import("trifinger_cameras");

    // import for Python bindings of ObjectPose
    pybind11::module::import("trifinger_object_tracking.py_object_tracker");

    create_sensor_bindings<TriCameraObjectObservation,
                           trifinger_cameras::TriCameraInfo>(m);

    pybind11::class_<TriCameraObjectObservation,
                     trifinger_cameras::TriCameraObservation>(
        m,
        "TriCameraObjectObservation",
        "Observation from the three cameras, including the estimated object "
        "pose provided by the integrated object tracker.")
        .def(pybind11::init<>())
        .def_readwrite(
            "cameras",
            &TriCameraObjectObservation::cameras,
            "List[~trifinger_cameras.camera.CameraObservation]: List "
            "of observations from cameras 'camera60', 'camera180' "
            "and 'camera300' (in this order)")
        .def_readwrite("object_pose",
                       &TriCameraObjectObservation::object_pose,
                       "~trifinger_object_tracking.py_object_tracker."
                       "ObjectPose: Estimated object pose.")
        .def_readwrite("filtered_object_pose",
                       &TriCameraObjectObservation::filtered_object_pose,
                       "~trifinger_object_tracking.py_object_tracker."
                       "ObjectPose: Filtered estimated object pose.");

#ifdef Pylon_FOUND
    pybind11::class_<TriCameraObjectTrackerDriver,
                     std::shared_ptr<TriCameraObjectTrackerDriver>,
                     SensorDriver<TriCameraObjectObservation,
                                  trifinger_cameras::TriCameraInfo>>(
        m, "TriCameraObjectTrackerDriver")
        .def(pybind11::init<const std::string&,
                            const std::string&,
                            const std::string&,
                            BaseCuboidModel::ConstPtr,
                            bool>(),
             pybind11::arg("camera1"),
             pybind11::arg("camera2"),
             pybind11::arg("camera3"),
             pybind11::arg("cube_model"),
             pybind11::arg("downsample_images") = true)
        .def(pybind11::init<const std::filesystem::path&,
                            const std::filesystem::path&,
                            const std::filesystem::path&,
                            BaseCuboidModel::ConstPtr,
                            bool>(),
             pybind11::arg("camera_calibration_file_1"),
             pybind11::arg("camera_calibration_file_2"),
             pybind11::arg("camera_calibration_file_3"),
             pybind11::arg("cube_model"),
             pybind11::arg("downsample_images") = true)
        .def(pybind11::init<robot_interfaces::SensorData<
                                trifinger_cameras::TriCameraObservation,
                                trifinger_cameras::TriCameraInfo>::Ptr,
                            BaseCuboidModel::ConstPtr,
                            bool>(),
             pybind11::arg("camera_data"),
             pybind11::arg("cube_model"),
             pybind11::arg("downsample_images") = true)
        .def("get_observation",
             &TriCameraObjectTrackerDriver::get_observation,
             "Get the latest observation from the three cameras.")
        .def("get_debug_image",
             &TriCameraObjectTrackerDriver::get_debug_image,
             pybind11::arg("fill_faces") = false,
             "Fetch an observation from the cameras and create a debug image.");
#endif

    pybind11::class_<PyBulletTriCameraObjectTrackerDriver,
                     std::shared_ptr<PyBulletTriCameraObjectTrackerDriver>,
                     SensorDriver<TriCameraObjectObservation,
                                  trifinger_cameras::TriCameraInfo>>(
        m, "PyBulletTriCameraObjectTrackerDriver")
        .def(pybind11::init<pybind11::object,
                            robot_interfaces::TriFingerTypes::BaseDataPtr,
                            bool>())
        .def("get_observation",
             &PyBulletTriCameraObjectTrackerDriver::get_observation);

    pybind11::class_<CubeVisualizer>(m, "CubeVisualizer")
        .def(pybind11::init<BaseCuboidModel::ConstPtr,
                            std::array<std::string, 3>>())
        .def("draw_cube",
             &CubeVisualizer::draw_cube,
             pybind11::arg("images"),
             pybind11::arg("object_pose"),
             pybind11::arg("fill") = true,
             pybind11::arg("opacity") = 0.5)
        .def("draw_circle",
             &CubeVisualizer::draw_circle,
             pybind11::arg("images"),
             pybind11::arg("object_pose"),
             pybind11::arg("fill") = true,
             pybind11::arg("opacity") = 0.5,
             pybind11::arg("scale") = 1.0);
}
