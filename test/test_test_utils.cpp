/**
 * @file
 * @brief Tests for the test utility functions
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>

#include "utils.hpp"

using namespace trifinger_object_tracking;

/* Python code for generating the test data:
 *
 *  from scipy.spatial.transform import Rotation
 *  import numpy as np
 *  orientations = {
 *      "base": Rotation.from_euler("x", 0),
 *      "x90": Rotation.from_euler("x", 90, degrees=True),
 *      "y90": Rotation.from_euler("y", 90, degrees=True),
 *      "ym90": Rotation.from_euler("y", -90, degrees=True),
 *      "z180": Rotation.from_euler("z", 180, degrees=True),
 *      "xy45": Rotation.from_euler("xy", [45, 45], degrees=True),
 *  }
 *  for name, rot in orientations.items():
 *      vec = rot.as_rotvec()
 *      print(f"cv::Vec3f {name}({vec[0]}, {vec[1]}, {vec[2]});")
 *  for name, rot in orientations.items():
 *      q = rot.as_quat()
 *      print(f"Eigen::Vector4d {name}({q[0]}, {q[1]}, {q[2]}, {q[3]});")
 */

TEST(TestTestUtils, get_rotation_error_rodrigues)
{
    cv::Vec3f base(0.0, 0.0, 0.0);
    cv::Vec3f x90(1.5707963267948963, 0.0, 0.0);
    cv::Vec3f y90(0.0, 1.5707963267948963, 0.0);
    cv::Vec3f ym90(0.0, -1.5707963267948963, 0.0);
    cv::Vec3f z180(0.0, 0.0, 3.141592653589793);

    EXPECT_FLOAT_EQ(get_rotation_error(base, base), 0.0f);
    EXPECT_FLOAT_EQ(get_rotation_error(x90, x90), 0.0f);
    EXPECT_FLOAT_EQ(get_rotation_error(base, x90), M_PI / 2);
    EXPECT_FLOAT_EQ(get_rotation_error(base, y90), M_PI / 2);
    EXPECT_FLOAT_EQ(get_rotation_error(base, ym90), M_PI / 2);
    EXPECT_FLOAT_EQ(get_rotation_error(base, z180), M_PI);
}

TEST(TestTestUtils, get_rotation_error_quaternion)
{
    Eigen::Vector4d base(0.0, 0.0, 0.0, 1.0);
    Eigen::Vector4d x90(0.7071067811865475, 0.0, 0.0, 0.7071067811865476);
    Eigen::Vector4d y90(0.0, 0.7071067811865475, 0.0, 0.7071067811865476);
    Eigen::Vector4d ym90(0.0, -0.7071067811865475, 0.0, 0.7071067811865476);
    Eigen::Vector4d z180(0.0, 0.0, 1.0, 6.123233995736766e-17);

    EXPECT_FLOAT_EQ(get_rotation_error(base, base), 0.0f);
    EXPECT_FLOAT_EQ(get_rotation_error(x90, x90), 0.0f);
    EXPECT_FLOAT_EQ(get_rotation_error(base, x90), M_PI / 2);
    EXPECT_FLOAT_EQ(get_rotation_error(base, y90), M_PI / 2);
    EXPECT_FLOAT_EQ(get_rotation_error(base, ym90), M_PI / 2);
    EXPECT_FLOAT_EQ(get_rotation_error(base, z180), M_PI);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
