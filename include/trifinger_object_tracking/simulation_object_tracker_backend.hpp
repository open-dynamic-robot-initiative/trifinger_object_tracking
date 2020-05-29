/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include "base_object_tracker_backend.hpp"

namespace trifinger_object_tracking
{
class SimulationObjectTrackerBackend : public BaseObjectTrackerBackend
{
public:
    SimulationObjectTrackerBackend(ObjectTrackerData::Ptr data)
        : BaseObjectTrackerBackend(data)
    {
    }

protected:
    ObjectPose update_pose() override;
};

}  // namespace trifinger_object_tracking
