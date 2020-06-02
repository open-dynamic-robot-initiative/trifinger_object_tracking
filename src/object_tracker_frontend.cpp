/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/object_tracker_frontend.hpp>

namespace trifinger_object_tracking
{
ObjectPose ObjectTrackerFrontend::get_pose(const time_series::Index t) const
{
    return (*data_->poses)[t];
}

ObjectPose ObjectTrackerFrontend::get_current_pose() const
{
    return data_->poses->newest_element();
}

time_series::Index ObjectTrackerFrontend::get_current_timeindex() const
{
    return data_->poses->newest_timeindex();
}

time_series::Index ObjectTrackerFrontend::get_oldest_timeindex() const
{
    return data_->poses->oldest_timeindex();
}

void ObjectTrackerFrontend::wait_until_timeindex(const time_series::Index t) const
{
    data_->poses->timestamp_ms(t);
}

bool ObjectTrackerFrontend::has_observations() const
{
    return !data_->poses->is_empty();
}

}  // namespace trifinger_object_tracking
