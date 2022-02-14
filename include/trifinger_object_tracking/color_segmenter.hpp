#pragma once
#include <math.h>

#include <chrono>
#include <future>
#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/types.hpp>

namespace trifinger_object_tracking
{
constexpr int XGB_NUM_CLASSES = 7;
constexpr int XGB_NUM_FEATURES = 6;

std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v1(
    std::array<float, XGB_NUM_FEATURES> &sample);

std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v2(
    std::array<float, XGB_NUM_FEATURES> &sample);

std::array<float, XGB_NUM_CLASSES> xgb_classify_cuboid_v2(
    std::array<float, XGB_NUM_FEATURES> &sample);

inline std::array<float, XGB_NUM_CLASSES> xgb_classify(
    const ColorModel color_model, std::array<float, XGB_NUM_FEATURES> &sample)
{
    switch (color_model)
    {
        case ColorModel::CUBE_V1:
            return xgb_classify_cube_v1(sample);
        case ColorModel::CUBE_V2:
            return xgb_classify_cube_v2(sample);
        case ColorModel::CUBOID_V2:
            return xgb_classify_cuboid_v2(sample);
        default:
            // this should never happen
            throw std::runtime_error("Unsupported color model");
    }
}

class ColorSegmenter
{
private:
    BaseCuboidModel::ConstPtr cube_model_;
    cv::Mat image_hsv_, image_bgr_;

    //! individual color segment mask
    std::array<cv::Mat, FaceColor::N_COLORS> masks_;

    std::vector<FaceColor> dominant_colors_;

    void xgboost_mask();

public:
    // constructor
    ColorSegmenter(BaseCuboidModel::ConstPtr cube_model);

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
