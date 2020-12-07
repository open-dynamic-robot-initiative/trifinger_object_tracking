/**
 * @file
 * @brief Tests for Image
 * @copyright Copyright (c) 2020, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>
#include <ament_index_cpp/get_package_share_directory.hpp>

#include <memory>
#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/color_segmenter.hpp>

using namespace trifinger_object_tracking;

/**
 * @brief Fixture for the backend tests.
 */
class TestImage : public ::testing::Test
{
protected:
    static constexpr int N_IMG = 2;
    static constexpr int N_COLORS = 6;

    enum Color
    {
        RED = 0,
        GREEN,
        BLUE,
        CYAN,
        MAGENTA,
        YELLOW
    };

    std::map<Color, std::string> color_names;

    cv::Mat masks[N_IMG][N_COLORS];
    std::shared_ptr<Image> image[N_IMG];

    void SetUp() override
    {
        color_names[RED] = "red";
        color_names[GREEN] = "green";
        color_names[BLUE] = "blue";
        color_names[CYAN] = "cyan";
        color_names[MAGENTA] = "magenta";
        color_names[YELLOW] = "yellow";

        std::string package_path = ament_index_cpp::get_package_share_directory(
            "trifinger_object_tracking");
        std::string data_dir = package_path + "/data/";
        std::string test_image_dir = package_path + "/test/images/";

        // load two test image with ground truth masks
        cv::Mat img;

        img = cv::imread(test_image_dir + "01_image.png");
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        image[0] = std::make_shared<Image>(img, data_dir);

        for (auto color : {Color::CYAN, Color::MAGENTA, Color::YELLOW})
        {
            std::string filename = "01_mask_" + color_names[color] + ".png";
            masks[0][color] =
                cv::imread(test_image_dir + filename, cv::IMREAD_GRAYSCALE);
        }
        for (auto color : {Color::RED, Color::GREEN, Color::BLUE})
        {
            masks[0][color] = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        }

        img = cv::imread(test_image_dir + "02_image.png");
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        image[1] = std::make_shared<Image>(img, data_dir);

        for (auto color : {Color::RED, Color::GREEN, Color::BLUE})
        {
            std::string filename = "02_mask_" + color_names[color] + ".png";
            masks[1][color] =
                cv::imread(test_image_dir + filename, cv::IMREAD_GRAYSCALE);
        }
        for (auto color : {Color::CYAN, Color::MAGENTA, Color::YELLOW})
        {
            masks[1][color] = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        }

        // verify all masks are loaded properly
        for (size_t i = 0; i < N_IMG; i++)
        {
            for (auto color : {Color::RED,
                               Color::GREEN,
                               Color::BLUE,
                               Color::CYAN,
                               Color::MAGENTA,
                               Color::YELLOW})
            {
                std::cout << "Verify mask " << i << "/" << color_names[color]
                          << std::endl;
                ASSERT_EQ(masks[i][color].rows, 540);
                ASSERT_EQ(masks[i][color].cols, 720);
                ASSERT_EQ(masks[i][color].channels(), 1);
            }
        }
    }
};

// TEST_F(TestImage, gmm_mask)
//{
//    // TODO the number of wrong pixels is pretty high, try to reduce it
//    constexpr int MAX_WRONG_PIXELS = 3800;
//
//    for (size_t i = 0; i < N_IMG; i++)
//    {
//        image[i]->gmm_mask();
//
//        for (auto color : {Color::RED,
//                           Color::GREEN,
//                           Color::BLUE,
//                           Color::CYAN,
//                           Color::MAGENTA,
//                           Color::YELLOW})
//        {
//            // std::cout << "color: " << color_names[color] << std::endl;
//            // cv::imshow("true", masks[i][color]);
//            // cv::imshow("predicted",
//            image[i]->get_mask(color_names[color]));
//            // cv::waitKey();
//
//            // then number of wrongly segmented pixels has to be less than
//            // MAX_WRONG_PIXELS
//            EXPECT_LT(cv::countNonZero(masks[i][color] !=
//                                       image[i]->get_mask(color_names[color])),
//                      MAX_WRONG_PIXELS);
//        }
//    }
//}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
