/**
 * @file
 * @brief Python bindings for the object tracker interface.
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <pybind11/eigen.h>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

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
        .def(pybind11::init<size_t>())
        .def_readwrite("timestamp", &ObjectPose::timestamp)
        .def_readwrite("position", &ObjectPose::position)
        .def_readwrite("orientation", &ObjectPose::orientation)
        .def_readwrite("confidence", &ObjectPose::confidence)
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
}
