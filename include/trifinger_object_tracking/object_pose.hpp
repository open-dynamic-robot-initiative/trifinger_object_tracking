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
        archive(cereal::make_nvp("t", timestamp),
                cereal::make_nvp("x", position[0]),
                cereal::make_nvp("y", position[1]),
                cereal::make_nvp("z", position[2]),
                cereal::make_nvp("roll", orientation[0]),
                cereal::make_nvp("pitch", orientation[1]),
                cereal::make_nvp("yaw", orientation[2]),
                CEREAL_NVP(confidence));
    }
};


}  // namespace trifinger_object_tracking
