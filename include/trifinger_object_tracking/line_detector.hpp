#pragma once
#include <math.h>
#include <armadillo>
#include <chrono>
#include <future>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/types.hpp>

namespace trifinger_object_tracking
{
class LineDetector
{
private:
    CubeModel cube_model_;
    cv::Mat image_hsv_, image_bgr_;

    std::array<ColorBounds, FaceColor::N_COLORS> color_bounds_;
    std::array<float, FaceColor::N_COLORS> gmm_thresholds_;

    //! individual color segment mask
    std::array<cv::Mat, FaceColor::N_COLORS> masks_;

    //! deflated masks. only defined for dominant colours
    std::array<cv::Mat, FaceColor::N_COLORS> deflated_masks_;

    //! pixel coordinates of the region of interest
    std::map<FaceColor, std::vector<cv::Point>> pixel_dataset_;
    //! total pixels with a particular color
    std::map<FaceColor, int> color_count_;
    std::array<arma::gmm_full, FaceColor::N_COLORS> segmentation_models_;
    std::chrono::high_resolution_clock::time_point start_, finish_;

    ColorEdgeLineList lines_;

    std::vector<FaceColor> dominant_colors_;

    void set_color_bounds();
    void load_segmentation_models(const std::string &model_directory);

    void clean_mask(
        FaceColor color,
        std::array<std::vector<int>, FaceColor::N_COLORS> &pixel_idx);

    void deflate_masks_of_dominant_colors();

    std::array<std::vector<cv::Point>, 2> get_front_line_pixels(
        FaceColor color1, FaceColor color2) const;

    void xgboost_mask();
    void gmm_mask();

    void find_dominant_colors(const unsigned int);

    std::vector<std::pair<FaceColor, FaceColor>> make_valid_combinations()
        const;

    void get_line_between_colors(FaceColor color1, FaceColor color2);

public:
    // constructor
    LineDetector(const CubeModel &cube_model,
                 const std::string &model_directory);

    /**
     * @brief Detect colours in the image and create segmentation masks.
     *
     * After calling this, dominant colours and segmentation masks are provided
     * by get_dominant_colors() and get_mask().
     *
     * @param image_bgr
     */
    void detect_colors(const cv::Mat &image_bgr);

    /**
     * @brief Detect cube edge lines in the given image.
     *
     * @param image_bgr
     *
     * @return
     */
    ColorEdgeLineList detect_lines(const cv::Mat &image_bgr);

    //! @brief Get mask of the specified color.
    cv::Mat get_mask(FaceColor color) const;

    //! @brief Get image visualizing the color segmentation.
    cv::Mat get_segmented_image() const;

    //! @brief Get image visualizing the front line pixels.
    cv::Mat get_front_line_image() const;

    //! @brief Get image visualizing the detected lines.
    cv::Mat get_image_lines() const;

    //! @brief Get the original image.
    cv::Mat get_image() const;

    std::vector<FaceColor> get_dominant_colors() const;
};

}  // namespace trifinger_object_tracking
