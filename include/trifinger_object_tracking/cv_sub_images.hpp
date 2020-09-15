/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <opencv2/opencv.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Combine multiple images of same size in grid.
 *
 * Creates an nxm grid of images, similar to subplots of matplotlib.
 */
class CvSubImages
{
public:
    /**
     * @param img_size Size of the single images.
     * @param rows Number of rows in the grid.
     * @param cols Number of columns in the grid.
     * @param border Size of the border between images in pixels.
     * @param background Background colour.
     */
    CvSubImages(cv::Size img_size,
                unsigned rows,
                unsigned cols,
                unsigned border = 5,
                cv::Scalar background = cv::Scalar(255, 255, 255));

    /**
     * @brief Set a sub-image.
     *
     * @param image The image.
     * @param row Row in which the image is shown.
     * @param col Column in which the image is shown.
     */
    void set_subimg(const cv::Mat &image, unsigned row, unsigned col);

    //! @brief Get the image grid.
    const cv::Mat &get_image() const;

private:
    cv::Size subimg_size_;
    unsigned border_;
    cv::Mat image_;
};
}  // namespace trifinger_object_tracking

