#include <math.h>

#include <algorithm>  // std::sort, std::stable_sort
#include <chrono>
#include <iostream>
#include <numeric>  // std::iota
#include <thread>
#include <trifinger_object_tracking/color_segmenter.hpp>
#include <trifinger_object_tracking/scoped_timer.hpp>
#include <typeinfo>

namespace trifinger_object_tracking
{
ColorSegmenter::ColorSegmenter(BaseCuboidModel::ConstPtr cube_model)
    : cube_model_(cube_model)
{
}

void ColorSegmenter::detect_colors(const cv::Mat &image_bgr)
{
    // TODO better solution than class members for images

    // blur the image to make colour classification easier
    cv::medianBlur(image_bgr, image_bgr_, 5);
    cv::cvtColor(image_bgr_, image_hsv_, cv::COLOR_BGR2HSV);

    xgboost_mask();
}

void ColorSegmenter::xgboost_mask()
{
    // map label index of the xgboost model to color
    constexpr std::array<FaceColor, FaceColor::N_COLORS> map_label_to_color = {
        FaceColor::BLUE,
        FaceColor::CYAN,
        FaceColor::GREEN,
        FaceColor::MAGENTA,
        FaceColor::RED,
        FaceColor::YELLOW,
    };

    // structuring element for denoising
    constexpr unsigned OPEN_RADIUS = 1;
    static const cv::Mat open_kernel = cv::getStructuringElement(
        cv::MORPH_ELLIPSE, cv::Size(2 * OPEN_RADIUS + 1, 2 * OPEN_RADIUS + 1));

    // initialize masks
    for (FaceColor color : cube_model_->get_colors())
    {
        masks_[color] =
            cv::Mat(image_bgr_.rows, image_bgr_.cols, CV_8UC1, cv::Scalar(0));
    }

    for (int r = 0; r < image_bgr_.rows; r += 1)
    {
        for (int c = 0; c < image_bgr_.cols; c += 1)
        {
            std::array<float, XGB_NUM_FEATURES> features;

            cv::Vec3b bgr = image_bgr_.at<cv::Vec3b>(r, c);
            cv::Vec3b hsv = image_hsv_.at<cv::Vec3b>(r, c);

            features[0] = static_cast<float>(bgr[0]);
            features[1] = static_cast<float>(bgr[1]);
            features[2] = static_cast<float>(bgr[2]);
            features[3] = static_cast<float>(hsv[0]);
            features[4] = static_cast<float>(hsv[1]);
            features[5] = static_cast<float>(hsv[2]);

            std::array<float, XGB_NUM_CLASSES> probabilities =
                xgb_classify(cube_model_->get_color_model(), features);

            auto max_elem =
                std::max_element(probabilities.begin(), probabilities.end());
            int label = max_elem - probabilities.begin();

            if (label > 0)
            {
                FaceColor color = map_label_to_color[label - 1];
                masks_[color].at<uint8_t>(r, c) = 255;
            }
        }
    }

    std::array<unsigned int, FaceColor::N_COLORS> color_counts;
    // post-process masks
    for (FaceColor color : cube_model_->get_colors())
    {
        // "open" image to get rid of single-pixel noise
        cv::morphologyEx(
            masks_[color], masks_[color], cv::MORPH_OPEN, open_kernel);

        // count number of pixels of each color
        color_counts[color] = cv::countNonZero(masks_[color]);
    }

    // we store the 3 colors with most pixels in dominant_colors_
    std::vector<unsigned int> colors_sorted_descending(FaceColor::N_COLORS);
    std::iota(
        colors_sorted_descending.begin(), colors_sorted_descending.end(), 0);
    std::sort(colors_sorted_descending.begin(),
              colors_sorted_descending.end(),
              [&color_counts](size_t i1, size_t i2) {
                  return color_counts[i1] > color_counts[i2];
              });

    dominant_colors_.resize(3);
    for (size_t i = 0; i < dominant_colors_.size(); i++)
    {
        dominant_colors_[i] = FaceColor(colors_sorted_descending[i]);
    }
}

cv::Mat ColorSegmenter::get_segmented_image() const
{
    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (FaceColor color : dominant_colors_)
    {
        auto rgb = cube_model_->get_rgb(color);
        // image is BGR, so swap R and B
        cv::Scalar color_bgr(rgb[2], rgb[1], rgb[0]);
        segmentation.setTo(color_bgr, masks_[color]);
    }
    return segmentation.clone();
}

cv::Mat ColorSegmenter::get_image() const
{
    return image_bgr_.clone();
}

cv::Mat ColorSegmenter::get_mask(FaceColor color) const
{
    return masks_[color];
}

std::vector<FaceColor> ColorSegmenter::get_dominant_colors() const
{
    return dominant_colors_;
}

}  // namespace trifinger_object_tracking
