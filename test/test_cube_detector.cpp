/**
 * @file
 * @brief Tests for CubeDetector
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/utils.hpp>

using namespace trifinger_object_tracking;

/**
 * @brief Fixture for the CubeDetector tests.
 */
class TestCubeDetector : public ::testing::Test
{
protected:
    typedef std::array<cv::Mat, CubeDetector::N_CAMERAS> ImageArray;

    ImageArray images_;
    std::array<std::string, CubeDetector::N_CAMERAS> camera_parameter_files_;
    std::array<trifinger_cameras::CameraParameters, CubeDetector::N_CAMERAS>
        camera_parameters_;

    void load_test_data(const std::string &object_name)
    {
        std::string package_path = ament_index_cpp::get_package_share_directory(
            "trifinger_object_tracking");
        std::string test_image_dir =
            package_path + "/test/images/pose_detection/" + object_name + "/";

        images_ = ImageArray({cv::imread(test_image_dir + "camera60.png"),
                              cv::imread(test_image_dir + "camera180.png"),
                              cv::imread(test_image_dir + "camera300.png")});

        camera_parameter_files_ =
            std::array<std::string, CubeDetector::N_CAMERAS>(
                {test_image_dir + "camera_calib_60.yml",
                 test_image_dir + "camera_calib_180.yml",
                 test_image_dir + "camera_calib_300.yml"});

        camera_parameters_ = trifinger_object_tracking::load_camera_parameters(
            camera_parameter_files_);
    }
};

TEST_F(TestCubeDetector, find_pose_cube_v1)
{
    auto cube_model = std::make_shared<const CubeV1Model>();
    load_test_data(cube_model->get_name());

    CubeDetector cube_detector(cube_model, camera_parameters_);
    // FIXME add tests for the multi-thread version
    ObjectPose pose = cube_detector.detect_cube_single_thread(images_);

    // FIXME the detected position is actually a bit off here.  The cube is
    // on the ground, so the height would need to be 0.0325.
    cv::Vec3f actual_position(-0.014, -0.024, 0.028);
    cv::Vec4f actual_orientation(
        0.58214203, 0.36977536, -0.61211132, 0.38690641);
    //= cv::Vec3f(1.48171, 0.941179, -1.55799);

    EXPECT_NEAR(pose.position[0], actual_position[0], 0.01);
    EXPECT_NEAR(pose.position[1], actual_position[1], 0.01);
    EXPECT_NEAR(pose.position[2], actual_position[2], 0.01);

    // FIXME What is a good way to compare orientation vectors?
    // EXPECT_NEAR(pose.orientation[0], actual_orientation[0], 0.01);
    // EXPECT_NEAR(pose.orientation[1], actual_orientation[1], 0.01);
    // EXPECT_NEAR(pose.orientation[2], actual_orientation[2], 0.01);
    // EXPECT_NEAR(pose.orientation[3], actual_orientation[2], 0.01);

    EXPECT_GT(pose.confidence, 0.8);
}

TEST_F(TestCubeDetector, find_pose_cube_v1_load_camera_params_from_files)
{
    auto cube_model = std::make_shared<const CubeV1Model>();
    load_test_data(cube_model->get_name());

    CubeDetector cube_detector(cube_model, camera_parameter_files_);
    ObjectPose pose = cube_detector.detect_cube_single_thread(images_);

    // FIXME the detected position is actually a bit off here.  The cube is
    // on the ground, so the height would need to be 0.0325.
    cv::Vec3f actual_position(-0.014, -0.024, 0.028);
    cv::Vec4f actual_orientation(
        0.58214203, 0.36977536, -0.61211132, 0.38690641);
    //= cv::Vec3f(1.48171, 0.941179, -1.55799);

    EXPECT_NEAR(pose.position[0], actual_position[0], 0.01);
    EXPECT_NEAR(pose.position[1], actual_position[1], 0.01);
    EXPECT_NEAR(pose.position[2], actual_position[2], 0.01);

    // FIXME What is a good way to compare orientation vectors?
    // EXPECT_NEAR(pose.orientation[0], actual_orientation[0], 0.01);
    // EXPECT_NEAR(pose.orientation[1], actual_orientation[1], 0.01);
    // EXPECT_NEAR(pose.orientation[2], actual_orientation[2], 0.01);
    // EXPECT_NEAR(pose.orientation[3], actual_orientation[2], 0.01);

    EXPECT_GT(pose.confidence, 0.8);
}

TEST_F(TestCubeDetector, find_pose_cube_v2)
{
    auto cube_model = std::make_shared<const CubeV2Model>();
    load_test_data(cube_model->get_name());

    CubeDetector cube_detector(cube_model, camera_parameters_);
    ObjectPose pose = cube_detector.detect_cube_single_thread(images_);

    // same issue with height as for version 1 (see above)
    cv::Vec3f actual_position(-0.063, -0.136, 0.025);
    cv::Vec4f actual_orientation(0.1192724, 0.6887785, -0.1423320, 0.7007851);

    EXPECT_NEAR(pose.position[0], actual_position[0], 0.01);
    EXPECT_NEAR(pose.position[1], actual_position[1], 0.01);
    EXPECT_NEAR(pose.position[2], actual_position[2], 0.01);

    // What is a good way to compare orientations?
    // EXPECT_NEAR(pose.orientation[0], actual_orientation[0], 0.01);
    // EXPECT_NEAR(pose.orientation[1], actual_orientation[1], 0.01);
    // EXPECT_NEAR(pose.orientation[2], actual_orientation[2], 0.01);
    // EXPECT_NEAR(pose.orientation[3], actual_orientation[2], 0.01);

    EXPECT_GT(pose.confidence, 0.8);
}

TEST_F(TestCubeDetector, find_pose_cuboid_2x2x8_v2)
{
    auto cube_model = std::make_shared<const Cuboid2x2x8V2Model>();
    load_test_data(cube_model->get_name());

    CubeDetector cube_detector(cube_model, camera_parameters_);
    ObjectPose pose = cube_detector.detect_cube_single_thread(images_);

    cv::Vec3f actual_position(-0.007, -0.008, 0.04);
    cv::Vec4f actual_orientation(0.67863974, -0.148312, -0.1528063, 0.70292382);

    EXPECT_NEAR(pose.position[0], actual_position[0], 0.01);
    EXPECT_NEAR(pose.position[1], actual_position[1], 0.01);
    EXPECT_NEAR(pose.position[2], actual_position[2], 0.01);

    // What is a good way to compare orientation vectors?
    // EXPECT_NEAR(pose.orientation[0], actual_orientation[0], 0.01);
    // EXPECT_NEAR(pose.orientation[1], actual_orientation[1], 0.01);
    // EXPECT_NEAR(pose.orientation[2], actual_orientation[2], 0.01);
    // EXPECT_NEAR(pose.orientation[3], actual_orientation[2], 0.01);

    EXPECT_GT(pose.confidence, 0.8);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
