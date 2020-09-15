#include <trifinger_object_tracking/cv_sub_images.hpp>

namespace trifinger_object_tracking
{
CvSubImages::CvSubImages(cv::Size img_size,
                         unsigned rows,
                         unsigned cols,
                         unsigned border,
                         cv::Scalar background)
    : subimg_size_(img_size), border_(border)
{
    unsigned width = img_size.width * cols + border * (cols + 1);
    unsigned height = img_size.height * rows + border * (rows + 1);

    image_ = cv::Mat(height, width, CV_8UC3, background);
}

void CvSubImages::set_subimg(const cv::Mat& image, unsigned row, unsigned col)
{
    unsigned offset_row = border_ + row * (subimg_size_.height + border_);
    unsigned offset_col = border_ + col * (subimg_size_.width + border_);

    cv::Mat subimg = image_(cv::Rect(
        offset_col, offset_row, subimg_size_.width, subimg_size_.height));

    image.copyTo(subimg);
}

const cv::Mat& CvSubImages::get_image() const
{
    return image_;
}

}  // namespace trifinger_object_tracking
