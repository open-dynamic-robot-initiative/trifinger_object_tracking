/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <pybind11/pybind11.h>

#include "base_object_tracker_backend.hpp"

namespace trifinger_object_tracking
{
/**
 * @brief Object Tracker Backend for Simulation.
 *
 * This implementation of the backend gets the object pose directly from
 * simulation.
 */
class SimulationObjectTrackerBackend : public BaseObjectTrackerBackend
{
public:
    /**
     * @brief Initialize.
     *
     * @param data  Instance of the ObjectTrackerData.
     * @param object  Python object that provides access to the object's pose.
     *     It is expected to have a method "get_pose()" that returns a tuple of
     *     object position ([x, y, z]) and orientation as quaternion
     *     ([x, y, z, w]).
     * @param real_time_mode  If true, the object pose will be updated at 33Hz.
     */
    SimulationObjectTrackerBackend(ObjectTrackerData::Ptr data,
                                   pybind11::object object,
                                   bool real_time_mode = true)
        : BaseObjectTrackerBackend(data),
          object_(object),
          real_time_mode_(real_time_mode)
    {
    }

protected:
    ObjectPose update_pose() override;

private:
    pybind11::object object_;
    bool real_time_mode_;
};

}  // namespace trifinger_object_tracking
