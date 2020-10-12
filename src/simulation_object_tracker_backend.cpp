/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/simulation_object_tracker_backend.hpp>

#include <chrono>
#include <thread>

#include <pybind11/eigen.h>
#include <pybind11/embed.h>

namespace trifinger_object_tracking
{
ObjectPose SimulationObjectTrackerBackend::update_pose()
{
    auto start_time = std::chrono::system_clock::now();

    ObjectPose pose;
    {
        pybind11::gil_scoped_acquire acquire;

        pybind11::tuple state = object_.attr("get_state")();

        pose.position = state[0].cast<Eigen::Vector3d>();
        pose.orientation = state[1].cast<Eigen::Vector4d>();
    }

    //auto current_time = std::chrono::system_clock::now();
    //pose.timestamp =
    //    std::chrono::duration<double>(current_time.time_since_epoch()).count();

    // we use perfect information from the simulation, so confidence is 1.0
    pose.confidence = 1.0;

    if (real_time_mode_)
    {
        // run at around 30 Hz
        using namespace std::chrono_literals;
        std::this_thread::sleep_until(start_time + 30ms);
    }
    // FIXME what would be a reasonable behaviour for non-real-time mode?

    return pose;
}

}  // namespace trifinger_object_tracking
