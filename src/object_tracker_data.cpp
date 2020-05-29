/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/object_tracker_data.hpp>

namespace trifinger_object_tracking
{
ObjectTrackerData::ObjectTrackerData(const std::string& shared_memory_id_prefix,
                                     bool is_master,
                                     size_t history_length)
{
    if (is_master)
    {
        // the master instance is in charge of cleaning the memory
        time_series::clear_memory(shared_memory_id_prefix);
    }

    const bool clean_on_destruction = is_master;

    this->poses =
        std::make_shared<time_series::MultiprocessTimeSeries<ObjectPose>>(
            shared_memory_id_prefix, history_length, clean_on_destruction);
}

}  // namespace trifinger_object_tracking
