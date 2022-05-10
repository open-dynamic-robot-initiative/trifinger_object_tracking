/**
 * @file
 * @brief Tests for cube_model.hpp
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>

#include <trifinger_object_tracking/cube_model.hpp>

using namespace trifinger_object_tracking;

TEST(TestCubeModel, get_model_by_name)
{
    std::vector<std::string> names = {
        "cube_v1", "cube_v2", "cube_v3", "cuboid_2x2x8_v2"};

    for (const std::string &name : names)
    {
        BaseCubeModel::ConstPtr model = get_model_by_name(name);
        EXPECT_EQ(model->get_name(), name);
    }
}

TEST(TestCubeModel, get_model_by_name_invalid)
{
    ASSERT_THROW(get_model_by_name("invalid"), std::runtime_error);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
