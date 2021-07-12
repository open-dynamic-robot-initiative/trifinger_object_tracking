#include <trifinger_object_tracking/xgboost_classifier_single_color_rgb.h>

namespace trifinger_object_tracking
{
cv::Mat segment_image(const cv::Mat &image_bgr)
{
    cv::Mat blurred_image_bgr;

    // initialise mask to same shape as input image but single-channel
    cv::Mat mask(image_bgr.rows, image_bgr.cols, CV_8UC1, cv::Scalar(0));

    // blur the image to make colour classification easier
    cv::medianBlur(image_bgr, blurred_image_bgr, 3);
    // cv::cvtColor(blurred_image_bgr, image_hsv_, cv::COLOR_BGR2HSV);

    // structuring element for denoising
    constexpr unsigned OPEN_RADIUS = 1;
    static const cv::Mat open_kernel = cv::getStructuringElement(
        cv::MORPH_ELLIPSE, cv::Size(2 * OPEN_RADIUS + 1, 2 * OPEN_RADIUS + 1));

    for (int r = 0; r < blurred_image_bgr.rows; r += 1)
    {
        for (int c = 0; c < blurred_image_bgr.cols; c += 1)
        {
            std::array<float, XGB_NUM_FEATURES> features;

            cv::Vec3b bgr = blurred_image_bgr.at<cv::Vec3b>(r, c);
            // cv::Vec3b hsv = image_hsv_.at<cv::Vec3b>(r, c);

            features[0] = static_cast<float>(bgr[0]);
            features[1] = static_cast<float>(bgr[1]);
            features[2] = static_cast<float>(bgr[2]);
            // features[3] = static_cast<float>(hsv[0]);
            // features[4] = static_cast<float>(hsv[1]);
            // features[5] = static_cast<float>(hsv[2]);

            std::array<float, XGB_NUM_CLASSES> probabilities =
                xgb_classify(features);

            // It seems that for a binary classification, the xgboost model only
            // has a single output.  So instead of looking for the class with
            // the highest value, we have to threshold this single value

            // TODO: what is the correct threshold?
            if (probabilities[0] > 0)
            {
                mask.at<uint8_t>(r, c) = 255;
            }
        }
    }

    // "open" image to get rid of single-pixel noise
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, open_kernel);

    return mask;
}

}  // namespace trifinger_object_tracking
