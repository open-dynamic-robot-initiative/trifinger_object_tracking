/**
 * @file
 * @brief Utility functions for the unit tests
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#pragma once

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>

namespace trifinger_object_tracking
{
//! @brief Compute angle between orientations v1, v2 given in Rodrigues
//!     representation.
inline float get_rotation_error(cv::Vec3f v1, cv::Vec3f v2)
{
    cv::Mat m1, m2;

    // convert to matrices
    cv::Rodrigues(v1, m1);
    cv::Rodrigues(v2, m2);

    // compute rotation between m1 and m2
    cv::Mat m_diff = m1 * m2.t();

    // Convert the diff rotation to Rodrigues.  The norm of the vector is the
    // magnitude of the rotation.
    cv::Vec3f v_diff;
    cv::Rodrigues(m_diff, v_diff);
    float angle = cv::norm(v_diff);

    return angle;
}

//! @brief Compute angle between orientations q1, q2 given as quaternions.
inline double get_rotation_error(Eigen::Vector4d q1, Eigen::Vector4d q2)
{
    constexpr int X = 0, Y = 1, Z = 2, W = 3;

    Eigen::Quaterniond quat1(q1[W], q1[X], q1[Y], q1[Z]);
    Eigen::Quaterniond quat2(q2[W], q2[X], q2[Y], q2[Z]);

    double angle = quat1.angularDistance(quat2);
    return angle;
}

}  // namespace trifinger_object_tracking
