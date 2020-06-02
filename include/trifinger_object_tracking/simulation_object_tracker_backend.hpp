/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <pybind11/pybind11.h>

#include "base_object_tracker_backend.hpp"

namespace trifinger_object_tracking
{
class SimulationObjectTrackerBackend : public BaseObjectTrackerBackend
{
public:
    SimulationObjectTrackerBackend(ObjectTrackerData::Ptr data,
                                   pybind11::object object)
        : BaseObjectTrackerBackend(data), object_(object)
    {
    }

protected:
    ObjectPose update_pose() override;

private:
    pybind11::object object_;
};

}  // namespace trifinger_object_tracking
