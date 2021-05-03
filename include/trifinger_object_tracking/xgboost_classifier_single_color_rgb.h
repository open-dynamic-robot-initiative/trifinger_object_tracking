#pragma once

#include <array>
#include <opencv2/opencv.hpp>

namespace trifinger_object_tracking
{
constexpr int XGB_NUM_CLASSES = 1;
constexpr int XGB_NUM_FEATURES = 3;

std::array<float, XGB_NUM_CLASSES> xgb_classify(
    const std::array<float, XGB_NUM_FEATURES> &sample);

/**
 * @brief Segment image using a binary pixel classifier
 *
 * @param image_bgr  The image in BGR format.
 *
 * @return Single-channel segmentation mask.
 */
cv::Mat segment_image(const cv::Mat &image_bgr);

}  // namespace trifinger_object_tracking
