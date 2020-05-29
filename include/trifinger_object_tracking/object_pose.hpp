/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <Eigen/Eigen>
#include <serialization_utils/cereal_eigen.hpp>

namespace trifinger_object_tracking
{
//! @brief Estimated pose of a tracked object.
struct ObjectPose
{
public:
    double timestamp;
    Eigen::Vector3d position;
    Eigen::Vector3d orientation;
    double confidence;

    //! For serialization with cereal.
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(timestamp, position, orientation, confidence);
    }
};

}  // namespace trifinger_object_tracking
