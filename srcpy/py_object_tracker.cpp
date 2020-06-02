#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eigen.h>
#include <pybind11/stl_bind.h>

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
        .def("__str__", &ObjectPose::to_string);

    pybind11::class_<ObjectTrackerData, ObjectTrackerData::Ptr>(
        m, "ObjectTrackerData")
        .def(pybind11::init<std::string, bool, size_t>(),
             "shared_memory_id_prefix"_a,
             "is_master"_a,
             "history_size"_a = 1000);

    pybind11::class_<SimulationObjectTrackerBackend>(
        m, "SimulationObjectTrackerBackend")
        .def(pybind11::init<ObjectTrackerData::Ptr, pybind11::object>(),
             "data"_a,
             "object"_a)
        .def("stop", &SimulationObjectTrackerBackend::stop)
        .def("store_buffered_data",
             &SimulationObjectTrackerBackend::store_buffered_data);

    pybind11::class_<ObjectTrackerFrontend>(m, "ObjectTrackerFrontend")
        .def(pybind11::init<ObjectTrackerData::Ptr>())
        .def("get_pose", &ObjectTrackerFrontend::get_pose)
        .def("get_current_pose",
             &ObjectTrackerFrontend::get_current_pose)
        .def("get_current_timeindex",
             &ObjectTrackerFrontend::get_current_timeindex)
        .def("get_oldest_timeindex",
             &ObjectTrackerFrontend::get_oldest_timeindex)
        .def("wait_until_timeindex",
             &ObjectTrackerFrontend::wait_until_timeindex)
        .def("has_observations", &ObjectTrackerFrontend::has_observations);
    // FIXME simplify names
}
