/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <array>

#include <trifinger_cameras/tricamera_observation.hpp>
#include <trifinger_object_tracking/object_pose.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Observation of three cameras + object tracking
 */
struct TriCameraObjectObservation
    : public trifinger_cameras::TriCameraObservation
{
    trifinger_object_tracking::ObjectPose object_pose = {};
    trifinger_object_tracking::ObjectPose filtered_object_pose = {};

    TriCameraObjectObservation() = default;

    TriCameraObjectObservation(
        const trifinger_cameras::TriCameraObservation& observation)
        : trifinger_cameras::TriCameraObservation(observation)
    {
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        trifinger_cameras::TriCameraObservation::serialize(archive);
        archive(object_pose, filtered_object_pose);
    }
};

}  // namespace trifinger_object_tracking
