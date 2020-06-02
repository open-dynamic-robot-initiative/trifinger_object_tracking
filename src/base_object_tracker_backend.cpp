/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/base_object_tracker_backend.hpp>

#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

namespace trifinger_object_tracking
{
BaseObjectTrackerBackend::~BaseObjectTrackerBackend()
{
    stop();
}

void BaseObjectTrackerBackend::stop()
{
    is_shutdown_requested_ = true;
    if (loop_thread_.joinable())
    {
        loop_thread_.join();
    }
}

void BaseObjectTrackerBackend::store_buffered_data(
    const std::string &filename) const
{
    if (is_running_)
    {
        throw std::runtime_error(
            "Cannot store buffer while running.  Call `stop()` first.");
    }

    std::vector<ObjectPose> data;

    if (!data_->poses->is_empty())
    {
        data.reserve(data_->poses->length());
        auto t_end = data_->poses->newest_timeindex();
        for (auto t = data_->poses->oldest_timeindex(); t <= t_end; t++)
        {
            data.push_back((*data_->poses)[t]);
        }
    }

    std::ofstream outfile(filename);
    cereal::JSONOutputArchive archive(
        outfile, cereal::JSONOutputArchive::Options::NoIndent());
    archive(cereal::make_nvp("object_poses", data));
    std::cout << "Stored " << data.size() << " observations." << std::endl;
}

void BaseObjectTrackerBackend::loop()
{
    is_running_ = true;

    while (!is_shutdown_requested_)
    {
        ObjectPose pose = update_pose();
        data_->poses->append(pose);
    }

    is_running_ = false;
}

}  // namespace trifinger_object_tracking

