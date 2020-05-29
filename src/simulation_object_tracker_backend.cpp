/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/simulation_object_tracker_backend.hpp>

namespace trifinger_object_tracking
{
ObjectPose SimulationObjectTrackerBackend::update_pose()
{
    static int time = 0;
    ObjectPose pose;
    pose.timestamp = time++;
    pose.confidence = 0.1;

    return pose;
}

}  // namespace trifinger_object_tracking

