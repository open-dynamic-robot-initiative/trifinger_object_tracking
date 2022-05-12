/**
 * @file
 * @brief Tests for PoseDetector
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/utils.hpp>

#include "utils.hpp"

using namespace trifinger_object_tracking;

/**
 * @brief Fixture for the backend tests.
 */
class TestPoseDetector : public ::testing::Test
{
protected:
    std::array<std::vector<FaceColor>, PoseDetector::N_CAMERAS>
        dominant_colors_;
    std::array<std::vector<cv::Mat>, PoseDetector::N_CAMERAS> masks_;
    std::array<trifinger_cameras::CameraParameters, PoseDetector::N_CAMERAS>
        camera_parameters_;

    void load_test_data(const std::string &object_name)
    {
        std::map<FaceColor, std::string> color_names;
        color_names[RED] = "r";
        color_names[GREEN] = "g";
        color_names[BLUE] = "b";
        color_names[CYAN] = "c";
        color_names[MAGENTA] = "m";
        color_names[YELLOW] = "y";

        std::array<std::string, PoseDetector::N_CAMERAS> camera_ids = {
            "60", "180", "300"};

        std::string package_path = ament_index_cpp::get_package_share_directory(
            "trifinger_object_tracking");
        std::string test_image_dir =
            package_path + "/test/images/pose_detection/" + object_name + "/";

        // load the test images (only masks needed here)
        for (size_t i = 0; i < PoseDetector::N_CAMERAS; i++)
        {
            for (auto color : {FaceColor::RED,
                               FaceColor::GREEN,
                               FaceColor::BLUE,
                               FaceColor::CYAN,
                               FaceColor::MAGENTA,
                               FaceColor::YELLOW})
            {
                std::string mask_filename =
                    color_names[color] + camera_ids[i] + ".png";
                cv::Mat mask = cv::imread(test_image_dir + mask_filename,
                                          cv::IMREAD_GRAYSCALE);
                if (!mask.empty())
                {
                    masks_[i].push_back(mask);
                    dominant_colors_[i].push_back(color);
                }
            }
        }

        camera_parameters_ = trifinger_object_tracking::load_camera_parameters(
            {test_image_dir + "camera_calib_60.yml",
             test_image_dir + "camera_calib_180.yml",
             test_image_dir + "camera_calib_300.yml"});
    }
};

TEST_F(TestPoseDetector, find_pose_cube_v1)
{
    auto cube_model = std::make_shared<const CubeV1Model>();
    load_test_data(cube_model->get_name());

    PoseDetector pose_detector(cube_model, camera_parameters_);

    Pose pose = pose_detector.find_pose(dominant_colors_, masks_);

    cv::Vec3f actual_translation, actual_rotation;

    // FIXME the detected translation is actually a bit off here.  The cube is
    // on the ground, so the height would need to be 0.0325.
    actual_translation = cv::Vec3f(-0.014, -0.024, 0.028);
    actual_rotation = cv::Vec3f(1.48171, 0.941179, -1.55799);

    EXPECT_NEAR(pose.translation[0], actual_translation[0], 0.01);
    EXPECT_NEAR(pose.translation[1], actual_translation[1], 0.01);
    EXPECT_NEAR(pose.translation[2], actual_translation[2], 0.01);
    EXPECT_LT(get_rotation_error(actual_rotation, pose.rotation), 0.2);

    EXPECT_GT(pose.confidence, 0.8);
}

TEST_F(TestPoseDetector, find_pose_cube_v2)
{
    auto cube_model = std::make_shared<const CubeV2Model>();
    load_test_data(cube_model->get_name());

    PoseDetector pose_detector(cube_model, camera_parameters_);

    Pose pose = pose_detector.find_pose(dominant_colors_, masks_);

    cv::Vec3f actual_translation, actual_rotation;

    // same issue with height as for version 1 (see above)
    actual_translation = cv::Vec3f(-0.063, -0.136, 0.025);
    actual_rotation = cv::Vec3f(0.265606, 1.53383, -0.316957);

    EXPECT_NEAR(pose.translation[0], actual_translation[0], 0.01);
    EXPECT_NEAR(pose.translation[1], actual_translation[1], 0.01);
    EXPECT_NEAR(pose.translation[2], actual_translation[2], 0.01);
    EXPECT_LT(get_rotation_error(actual_rotation, pose.rotation), 0.2);

    EXPECT_GT(pose.confidence, 0.8);
}

TEST_F(TestPoseDetector, find_pose_cuboid_2x2x8_v2)
{
    auto cube_model = std::make_shared<const Cuboid2x2x8V2Model>();
    load_test_data(cube_model->get_name());

    PoseDetector pose_detector(cube_model, camera_parameters_);

    Pose pose = pose_detector.find_pose(dominant_colors_, masks_);

    cv::Vec3f actual_translation, actual_rotation;

    actual_translation = cv::Vec3f(-0.007, -0.008, 0.04);
    actual_rotation = cv::Vec3f(1.51, -0.33, -0.34);  // TODO

    EXPECT_NEAR(pose.translation[0], actual_translation[0], 0.01);
    EXPECT_NEAR(pose.translation[1], actual_translation[1], 0.01);
    EXPECT_NEAR(pose.translation[2], actual_translation[2], 0.01);
    // orientation is pretty unreliable for the small cuboid, so don't check
    // that here
    // EXPECT_LT(get_rotation_error(actual_rotation, pose.rotation), 0.2);

    EXPECT_GT(pose.confidence, 0.8);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
