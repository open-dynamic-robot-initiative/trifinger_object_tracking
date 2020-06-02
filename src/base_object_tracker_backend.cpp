/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <trifinger_object_tracking/base_object_tracker_backend.hpp>

#include <fstream>

#include <cereal/archives/binary.hpp>
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


/**
 * @brief Store the content of the time series buffer to a file.
 *
 * @param filename  Path to the file.  If it already exists, it will be
 *                  overwritten.
 */
void BaseObjectTrackerBackend::store_buffered_data(
    const std::string &filename) const
{
    std::vector<ObjectPose> data;  // TODO use array instead?
    data.reserve(data_->poses->length());
    for (auto t = data_->poses->oldest_timeindex();
         t <= data_->poses->newest_timeindex();
         t++)
    {
        data.push_back((*data_->poses)[t]);
    }

    std::ofstream outfile(filename);
    //cereal::JSONOutputArchive archive(outfile);
    // FIXME log should not be binary
    cereal::BinaryOutputArchive archive(outfile);
    archive(data);
}

void BaseObjectTrackerBackend::loop()
{
    while (!is_shutdown_requested_)
    {
        ObjectPose pose = update_pose();
        data_->poses->append(pose);
    }
}

}  // namespace trifinger_object_tracking

