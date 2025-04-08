/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#pragma once

#include <Eigen/Eigen>
#include <cereal/archives/json.hpp>
#include <serialization_utils/cereal_eigen.hpp>

namespace trifinger_object_tracking
{
//! @brief Estimated pose of a tracked object.
struct ObjectPose
{
public:
    //! Position (x, y, z)
    Eigen::Vector3d position;
    //! Orientation quaternion (x, y, z, w)
    Eigen::Vector4d orientation;
    //! Confidence of the accuracy of the given pose.  Ranges from 0 to 1.
    double confidence = 0.0;

    ObjectPose() = default;

    // construct pose from a homogeneous 4x4 transformation matrix
    ObjectPose(const Eigen::Matrix4d& matrix, double confidence = 0.0)
        : confidence(confidence)
    {
        position = matrix.block<3, 1>(0, 3);
        Eigen::Quaterniond q(matrix.block<3, 3>(0, 0));
        orientation[0] = q.x();
        orientation[1] = q.y();
        orientation[2] = q.z();
        orientation[3] = q.w();
    }

    //! Get the orientation as Eigen::Quaterniond.
    Eigen::Quaterniond quaternion() const
    {
        constexpr int X = 0, Y = 1, Z = 2, W = 3;
        return Eigen::Quaterniond(
            orientation[W], orientation[X], orientation[Y], orientation[Z]);
    }

    //! Get the pose as Eigen::Affine3d.
    Eigen::Affine3d affine() const
    {
        return Eigen::Translation3d(position) * quaternion();
    }

    //! For serialization with cereal.
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("px", position[0]),
                cereal::make_nvp("py", position[1]),
                cereal::make_nvp("pz", position[2]),
                cereal::make_nvp("qx", orientation[0]),
                cereal::make_nvp("qy", orientation[1]),
                cereal::make_nvp("qz", orientation[2]),
                cereal::make_nvp("qw", orientation[3]),
                CEREAL_NVP(confidence));
    }

    std::string to_string()
    {
        // use cereal JSON archive to get a nice string representation
        std::stringstream ss;
        {
            cereal::JSONOutputArchive archive(ss);
            this->serialize(archive);
        }

        return ss.str();
    }
};

}  // namespace trifinger_object_tracking
