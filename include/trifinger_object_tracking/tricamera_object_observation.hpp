/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <trifinger_cameras/camera_observation.hpp>
#include <trifinger_object_tracking/object_pose.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Observation of three cameras + object tracking
 */
struct TriCameraObjectObservation
{
    std::array<trifinger_cameras::CameraObservation, 3> cameras;

    trifinger_object_tracking::ObjectPose object_pose;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(cameras, object_pose);
    }
};

}  // namespace trifinger_object_tracking
