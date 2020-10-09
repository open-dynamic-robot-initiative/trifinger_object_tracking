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
    Eigen::Vector3d position;
    Eigen::Vector4d orientation;
    double confidence;

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
