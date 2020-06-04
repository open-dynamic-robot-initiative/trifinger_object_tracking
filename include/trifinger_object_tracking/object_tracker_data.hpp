/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <memory>
#include <string>

#include <time_series/multiprocess_time_series.hpp>

#include "object_pose.hpp"

namespace trifinger_object_tracking
{
//! Wrapper for the time series of the object tracker data.
class ObjectTrackerData
{
public:
    typedef std::shared_ptr<ObjectTrackerData> Ptr;

    ObjectTrackerData(const std::string& shared_memory_id_prefix,
                      bool is_master,
                      size_t history_length = 1000);

    std::shared_ptr<time_series::TimeSeriesInterface<ObjectPose>> poses;
};

}  // namespace trifinger_object_tracking
