/**
 * @file
 * @brief Python bindings for the object tracker interface.
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <pybind11/eigen.h>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <pybind11_opencv/cvbind.hpp>

#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/fake_object_tracker_backend.hpp>
#include <trifinger_object_tracking/object_pose.hpp>
#include <trifinger_object_tracking/object_tracker_data.hpp>
#include <trifinger_object_tracking/object_tracker_frontend.hpp>
#include <trifinger_object_tracking/simulation_object_tracker_backend.hpp>


using namespace pybind11::literals;
using namespace trifinger_object_tracking;

PYBIND11_MODULE(py_object_tracker, m)
{
    pybind11::class_<ObjectPose>(m, "ObjectPose")
        .def(pybind11::init<>())
        .def_readwrite("position",
                       &ObjectPose::position,
                       "Position (x, y, z) in the world frame.")
        .def_readwrite(
            "orientation",
            &ObjectPose::orientation,
            "Orientation quaternion (x, y, z, w) in the world frame.")
        .def_readwrite(
            "confidence",
            &ObjectPose::confidence,
            "Confidence measure of the object tracker.  Between 0 and 1.")
        .def("__str__",
             &ObjectPose::to_string,
             pybind11::call_guard<pybind11::gil_scoped_release>());

    pybind11::class_<ObjectTrackerData, ObjectTrackerData::Ptr>(m, "Data").def(
        pybind11::init<std::string, bool, size_t>(),
        "shared_memory_id_prefix"_a,
        "is_master"_a,
        "history_size"_a = 1000);

    pybind11::class_<SimulationObjectTrackerBackend>(m, "SimulationBackend")
        .def(pybind11::init<ObjectTrackerData::Ptr, pybind11::object, bool>(),
             "data"_a,
             "object"_a,
             "real_time_mode"_a = true)
        .def("stop",
             &SimulationObjectTrackerBackend::stop,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("store_buffered_data",
             &SimulationObjectTrackerBackend::store_buffered_data,
             pybind11::call_guard<pybind11::gil_scoped_release>());

    pybind11::class_<FakeObjectTrackerBackend>(m, "FakeBackend")
        .def(pybind11::init<ObjectTrackerData::Ptr>(), "data"_a)
        .def("stop",
             &FakeObjectTrackerBackend::stop,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("store_buffered_data",
             &FakeObjectTrackerBackend::store_buffered_data,
             pybind11::call_guard<pybind11::gil_scoped_release>());

    pybind11::class_<ObjectTrackerFrontend>(m, "Frontend")
        .def(pybind11::init<ObjectTrackerData::Ptr>())
        .def("get_pose",
             &ObjectTrackerFrontend::get_pose,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("get_current_pose",
             &ObjectTrackerFrontend::get_current_pose,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("get_current_timeindex",
             &ObjectTrackerFrontend::get_current_timeindex,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("get_oldest_timeindex",
             &ObjectTrackerFrontend::get_oldest_timeindex,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("get_timestamp_ms",
             &ObjectTrackerFrontend::get_timestamp_ms,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("wait_until_timeindex",
             &ObjectTrackerFrontend::wait_until_timeindex,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("has_observations",
             &ObjectTrackerFrontend::has_observations,
             pybind11::call_guard<pybind11::gil_scoped_release>());

    pybind11::class_<CubeDetector>(m, "CubeDetector")
        .def(pybind11::init<
             const std::array<std::string, CubeDetector::N_CAMERAS>>())
        .def("detect_cube_single_thread",
             &CubeDetector::detect_cube_single_thread,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("detect_cube",
             &CubeDetector::detect_cube,
             pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("create_debug_image",
             &CubeDetector::create_debug_image,
             "fill_faces"_a = false,
             pybind11::call_guard<pybind11::gil_scoped_release>());

    m.def("create_trifingerpro_cube_detector",
          &create_trifingerpro_cube_detector,
          "Create a CubeDetector for TriFingerPro robot, automatically loading "
          "the local camera calibration.",
          pybind11::call_guard<pybind11::gil_scoped_release>());
}
