/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include "base_object_tracker_backend.hpp"

namespace trifinger_object_tracking
{
/**
 * @brief Fake Object Tracker Backend for Testing.
 *
 * This implementation of the backend returns fake poses (meant for testing the
 * interface without real object tracker running).
 */
class FakeObjectTrackerBackend : public BaseObjectTrackerBackend
{
public:
    using BaseObjectTrackerBackend::BaseObjectTrackerBackend;

protected:
    ObjectPose update_pose() override;
};

}  // namespace trifinger_object_tracking

