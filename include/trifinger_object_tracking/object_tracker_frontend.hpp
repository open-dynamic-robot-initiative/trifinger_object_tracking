/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <time_series/interface.hpp>

#include "object_pose.hpp"
#include "object_tracker_data.hpp"

namespace trifinger_object_tracking
{
//! @brief Frontend to access the object tracker data.
class ObjectTrackerFrontend
{
public:
    ObjectTrackerFrontend(ObjectTrackerData::Ptr data) : data_(data)
    {
    }

    //! @brief Get the object pose at time index t.
    ObjectPose get_pose(const time_series::Index t) const;

    //! @brief Get the latest object pose.
    ObjectPose get_current_pose() const;

    //! @brief Get the index of the current time step.
    time_series::Index get_current_timeindex() const;

    //! @brief
    time_series::Index get_oldest_timeindex() const;

    //! @brief Wait until time index t is reached.
    void wait_until_timeindex(const time_series::Index t) const;

    //! @brief Returns true if there are observations in the time series.
    bool has_observations() const;

private:
    ObjectTrackerData::Ptr data_;
};

}  // namespace trifinger_object_tracking
