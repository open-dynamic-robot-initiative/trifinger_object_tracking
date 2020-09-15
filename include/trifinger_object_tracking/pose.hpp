#pragma once
#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/image.hpp>
#include <Eigen/Geometry>

#include <opencv2/core/eigen.hpp>


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
    std::map<FaceColor, int> face_normals_v_;
    cv::Mat reference_vector_normals_;
    Stats position_;
    Stats orientation_;
    bool initialisation_phase_ = true;
    bool continuous_estimation_ = false;
    cv::Point3f best_position_, best_orientation_;
    float best_cost_;
    std::vector<cv::Mat> pos_cams_w_frame_;

public:
    Pose(const std::vector<Image>&);

    void cross_entropy_method();

    std::vector<float> cost_function(std::vector<cv::Point3f>,
                                     std::vector<cv::Point3f>);

    void find_pose();

    std::vector<cv::Point3f> random_normal(cv::Point3f,
                                           cv::Point3f,
                                           int rows,
                                           int cols,
                                           std::string bounds_for="");

    std::vector<cv::Point3f> random_uniform(cv::Point3f lower_bound,
                                            cv::Point3f upper_bound,
                                            int rows,
                                            int cols);

    std::vector<cv::Point3f> sample_random_so3_rotvecs(int number_of_particles);

    cv::Point3f power(cv::Point3f, float);

    cv::Point3f mean(std::vector<cv::Point3f>);

    cv::Point3f var(std::vector<cv::Point3f>);

    cv::Mat getPoseMatrix(cv::Point3f, cv::Point3f);

    std::vector<std::vector<cv::Point2f>> projected_points_;

    cv::Mat _get_face_normals_cost(std::vector<cv::Mat> proposed_orientation_matrices);

    cv::Mat _cost_of_out_of_bounds_projection(std::vector<cv::Mat> projected_points);

    void initialise_pos_cams_w_frame();
};

}  // namespace trifinger_object_tracking
