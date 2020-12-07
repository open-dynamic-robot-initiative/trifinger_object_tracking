/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/fake_object_tracker_backend.hpp>

#include <chrono>
#include <thread>

namespace trifinger_object_tracking
{
ObjectPose FakeObjectTrackerBackend::update_pose()
{
    static double x = 0;

    auto start_time = std::chrono::system_clock::now();

    ObjectPose pose;
    pose.position << x, x * 2, x * 3;
    pose.orientation << x / 2, x / 3, x / 4, x / 5;

    // auto current_time = std::chrono::system_clock::now();
    // pose.timestamp =
    //    std::chrono::duration<double>(current_time.time_since_epoch()).count();

    // These are nonesense values, so confidence is zero :)
    pose.confidence = 0.0;

    // run at around 30 Hz
    using namespace std::chrono_literals;
    std::this_thread::sleep_until(start_time + 30ms);

    return pose;
}

}  // namespace trifinger_object_tracking
