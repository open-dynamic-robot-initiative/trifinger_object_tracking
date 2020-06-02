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
class ObjectTrackerFrontend
{
public:
    typedef time_series::Index TimeIndex;

    ObjectTrackerFrontend(ObjectTrackerData::Ptr data) : data_(data)
    {
    }

    ObjectPose get_pose(const TimeIndex t) const
    {
        return (*data_->poses)[t];
    }

    ObjectPose get_current_pose() const
    {
        return data_->poses->newest_element();
    }

    TimeIndex get_current_timeindex() const
    {
        return data_->poses->newest_timeindex();
    }

    TimeIndex get_oldest_timeindex() const
    {
        return data_->poses->oldest_timeindex();
    }

    void wait_until_timeindex(const TimeIndex t) const
    {
        data_->poses->timestamp_ms(t);
    }

    bool has_observations() const
    {
        return !data_->poses->is_empty();
    }

private:
    ObjectTrackerData::Ptr data_;
};

}  // namespace trifinger_object_tracking
