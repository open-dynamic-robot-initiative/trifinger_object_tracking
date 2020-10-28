#pragma once
#include <math.h>
#include <chrono>
#include <future>
#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/types.hpp>

namespace trifinger_object_tracking
{
class ColorSegmenter
{
private:
    CubeModel cube_model_;
    cv::Mat image_hsv_, image_bgr_;

    //! individual color segment mask
    std::array<cv::Mat, FaceColor::N_COLORS> masks_;

    std::vector<FaceColor> dominant_colors_;

    void xgboost_mask();

public:
    // constructor
    ColorSegmenter(const CubeModel &cube_model);

    /**
     * @brief Detect colours in the image and create segmentation masks.
     *
     * After calling this, dominant colours and segmentation masks are provided
     * by get_dominant_colors() and get_mask().
     *
     * @param image_bgr
     */
    void detect_colors(const cv::Mat &image_bgr);

    //! @brief Get mask of the specified color.
    cv::Mat get_mask(FaceColor color) const;

    //! @brief Get image visualizing the color segmentation.
    cv::Mat get_segmented_image() const;

    //! @brief Get the original image.
    cv::Mat get_image() const;

    std::vector<FaceColor> get_dominant_colors() const;
};

}  // namespace trifinger_object_tracking
