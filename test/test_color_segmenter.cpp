/**
 * @file
 * @brief Tests for ColorSegmenter
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>
#include <ament_index_cpp/get_package_share_directory.hpp>

#include <memory>
#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/color_segmenter.hpp>
#include <trifinger_object_tracking/cube_model.hpp>

using namespace trifinger_object_tracking;

/**
 * @brief Fixture for the backend tests.
 */
class TestColorSegmenter : public ::testing::Test
{
protected:
    static constexpr int N_IMG = 2;
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
                                     "/test/images/segmentation/object_v" +
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

TEST_F(TestColorSegmenter, get_mask)
{
    ColorSegmenter color_segmenter(cube_model_);

    //! Ratio of pixels in the image that are allowed to be wrongly classified
    //! in order for the test to succeed.
    constexpr float MAX_WRONG_PIXEL_RATIO = 0.005;

    for (size_t i = 0; i < N_IMG; i++)
    {
        color_segmenter.detect_colors(images_[i]);

        // check segmentation masks
        for (auto color : {FaceColor::RED,
                           FaceColor::GREEN,
                           FaceColor::BLUE,
                           FaceColor::CYAN,
                           FaceColor::MAGENTA,
                           FaceColor::YELLOW})
        {
            cv::Mat segmentation_mask = color_segmenter.get_mask(color);

            int num_misclassified_pixels =
                cv::countNonZero(masks_[i][color] != segmentation_mask);

            float mis_ratio = static_cast<float>(num_misclassified_pixels) /
                              static_cast<float>(images_[i].total());

            // then number of wrongly segmented pixels has to be less than
            // MAX_WRONG_PIXEL_RATIO
            EXPECT_LT(mis_ratio, MAX_WRONG_PIXEL_RATIO);
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
