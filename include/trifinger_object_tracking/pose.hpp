#pragma once
#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/image.hpp>

namespace trifinger_object_tracking
{
struct Stats
{
    cv::Point3f lower_bound;
    cv::Point3f upper_bound;
    cv::Point3f mean;
    cv::Point3f variance;
    cv::Point3f prev;
};

class Pose
{
    std::vector<Image> image_objects_;
    cv::Mat camera_matrix_;       // camera matrix
    cv::Mat distortion_coeffs_;   // distortion coeffs
    cv::Mat rotation_matrix_;     // rotation matrix
    cv::Mat translation_matrix_;  // traslation matrixtranslation matrix
    cv::Mat reference_center_Point_3d_;
    Stats position_;
    Stats orientation_;

public:
    Pose(std::vector<Image>);

    void cross_entropy_method();

    std::vector<float> cost_function(std::vector<cv::Point3f>,
                                     std::vector<cv::Point3f>);

    void find_pose();

    std::vector<cv::Point3f> random_normal(cv::Point3f,
                                           cv::Point3f,
                                           int rows,
                                           int cols);

    cv::Point3f power(cv::Point3f, float);

    cv::Point3f mean(std::vector<cv::Point3f>);

    cv::Point3f var(std::vector<cv::Point3f>);

    cv::Mat getPoseMatrix(cv::Point3f, cv::Point3f);

    std::vector<std::vector<cv::Point2f>> projected_points_;
};

}  // namespace trifinger_object_tracking
