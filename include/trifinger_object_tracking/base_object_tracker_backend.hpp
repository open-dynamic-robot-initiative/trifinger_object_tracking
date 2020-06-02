/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "object_pose.hpp"
#include "object_tracker_data.hpp"

namespace trifinger_object_tracking
{
/**
 * @brief Base class for object tracker.
 *
 * Implements the logic of putting the object pose to the time series.  The
 * method `update_pose()` which retrieves the actual pose, needs to be
 * implemented by the derived class.
 */
class BaseObjectTrackerBackend
{
public:
    BaseObjectTrackerBackend(ObjectTrackerData::Ptr data)
        : data_(data), is_shutdown_requested_(false)
    {
        loop_thread_ = std::thread(&BaseObjectTrackerBackend::loop, this);
    }

    ~BaseObjectTrackerBackend();

    void stop();

    /**
     * @brief Store the content of the time series buffer to a file.
     *
     * @param filename  Path to the file.  If it already exists, it will be
     *                  overwritten.
     */
    void store_buffered_data(const std::string &filename) const;

protected:
    virtual ObjectPose update_pose() = 0;

private:
    ObjectTrackerData::Ptr data_;
    std::atomic<bool> is_running_;
    std::atomic<bool> is_shutdown_requested_;
    std::thread loop_thread_;

    void loop();
};

}  // namespace trifinger_object_tracking
