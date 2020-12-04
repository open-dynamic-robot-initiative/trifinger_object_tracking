/**
 * @file
 * @brief Tests for PoseDetector
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>
#include <ament_index_cpp/get_package_share_directory.hpp>

#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/color_segmenter.hpp>
#include <trifinger_object_tracking/cube_model.hpp>

using namespace trifinger_object_tracking;

/**
 * @brief Fixture for the backend tests.
 */
class TestPoseDetector: public ::testing::Test
{
protected:
    static constexpr int N_COLORS = 6;

    std::map<FaceColor, std::string> color_names_;

    cv::Mat masks_[N_IMG][N_COLORS];
    cv::Mat images_[N_IMG];

    CubeModel cube_model_;

    void SetUp() override
    {
        color_names_[RED] = "red";
        color_names_[GREEN] = "green";
        color_names_[BLUE] = "blue";
        color_names_[CYAN] = "cyan";
        color_names_[MAGENTA] = "magenta";
        color_names_[YELLOW] = "yellow";

        std::string package_path = ament_index_cpp::get_package_share_directory(
            "trifinger_object_tracking");
        std::string test_image_dir = package_path +
                                     "/test/images/pose_detection/object_v" +
                                     std::to_string(OBJECT_VERSION) + "/";

        // load test image with ground truth masks
        for (int i = 0; i < N_IMG; i++)
        {
            std::string prefix = "0" + std::to_string(i + 1) + "_";

            std::string image_filename = test_image_dir + prefix + "image.png";
            images_[i] = cv::imread(image_filename);
            ASSERT_FALSE(images_[i].empty());

            for (auto color : {FaceColor::RED,
                               FaceColor::GREEN,
                               FaceColor::BLUE,
                               FaceColor::CYAN,
                               FaceColor::MAGENTA,
                               FaceColor::YELLOW})
            {
                std::string mask_filename =
                    prefix + "mask_" + color_names_[color] + ".png";
                cv::Mat mask = cv::imread(test_image_dir + mask_filename,
                                          cv::IMREAD_GRAYSCALE);
                if (mask.empty())
                {
                    mask = cv::Mat::zeros(
                        images_[i].rows, images_[i].cols, CV_8UC1);
                }

                masks_[i][color] = mask;
            }
        }

        // verify all masks are loaded properly
        for (size_t i = 0; i < N_IMG; i++)
        {
            for (auto color : {FaceColor::RED,
                               FaceColor::GREEN,
                               FaceColor::BLUE,
                               FaceColor::CYAN,
                               FaceColor::MAGENTA,
                               FaceColor::YELLOW})
            {
                // std::cout << "Verify mask " << i << "/"
                //           << color_names_[color] << std::endl;
                ASSERT_EQ(masks_[i][color].rows, images_[i].rows);
                ASSERT_EQ(masks_[i][color].cols, images_[i].cols);
                ASSERT_EQ(masks_[i][color].channels(), 1);
            }
        }
    }
};


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

