#pragma once
#include <Eigen/Geometry>

#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>

#include <trifinger_cameras/camera_parameters.hpp>

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/types.hpp>

namespace trifinger_object_tracking
{
struct Stats
{
    cv::Vec3f lower_bound;
    cv::Vec3f upper_bound;
    cv::Vec3f mean;
    cv::Vec3f variance;
    cv::Vec3f prev;
};

cv::Mat getPoseMatrix(cv::Point3f, cv::Point3f);

class PoseDetector
{
public:
    //! Number of cameras
    static constexpr unsigned int N_CAMERAS = 3;

    PoseDetector(const CubeModel &cube_model,
                 const std::array<trifinger_cameras::CameraParameters,
                                  N_CAMERAS> &camera_parameters);

    Pose find_pose(
        const std::array<ColorEdgeLineList, N_CAMERAS> &lines,
        const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
        const std::array<std::vector<cv::Mat>, N_CAMERAS> &masks);

    // TODO which points are projected?
    std::vector<std::vector<cv::Point2f>> get_projected_points() const;

    bool is_face_visible(FaceColor color,
                         unsigned int camera_idx,
                         const cv::Affine3f &cube_pose_world,
                         float *out_dot_product = nullptr) const;

    /**
     * @brief Get corner indices of the visible faces.
     *
     * Determines which faces of the object is visible to the camera and returns
     * the corner indices of these faces.
     *
     * @param camera_idx Index of the camera.
     * @param cube_pose_world Pose of the cube in the world frame.
     *
     * @return For each visible face a pair of the face color and the list of
     *     the corner indices of the four corners of that face.
     */
    std::vector<std::pair<FaceColor, std::array<unsigned int, 4>>>
    get_visible_faces(unsigned int camera_idx,
                      const cv::Affine3f &cube_pose_world) const;

    /**
     * @brief Get corner indices of the visible faces.
     *
     * Overloaded version that used the last detected pose for the cube.
     * Call find_pose() first, otherwise the result is undefined!
     *
     * @param camera_idx Index of the camera.
     *
     * @return For each visible face a pair of the face color and the list of
     *     the corner indices of the four corners of that face.
     */
    std::vector<std::pair<FaceColor, std::array<unsigned int, 4>>>
    get_visible_faces(unsigned int camera_idx) const;

private:
    CubeModel cube_model_;
    std::array<ColorEdgeLineList, N_CAMERAS> lines_;

    std::array<cv::Mat, N_CAMERAS> camera_matrices_;
    std::array<cv::Mat, N_CAMERAS> distortion_coeffs_;
    std::array<cv::Vec3d, N_CAMERAS> camera_orientations_;
    std::array<cv::Vec3d, N_CAMERAS> camera_translations_;

    cv::Mat corners_at_origin_in_world_frame_;
    cv::Mat reference_vector_normals_;
    Stats position_;
    Stats orientation_;
    bool initialisation_phase_ = true;
    bool continuous_estimation_ = false;
    cv::Vec3f best_position_, best_orientation_;
    float best_cost_;
    std::vector<cv::Mat> pos_cams_w_frame_;

    void cross_entropy_method(
        const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
        const std::array<std::vector<cv::Mat>, N_CAMERAS> &masks);

    std::vector<float> cost_function(
        const std::vector<cv::Vec3f> &tvecs,
        const std::vector<cv::Vec3f> &rvecs,
        const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
        const std::array<std::vector<cv::Mat>, N_CAMERAS> &masks,
        unsigned int iteration);

    std::vector<float> cost_function__(
        const std::vector<cv::Vec3f> &tvecs,
        const std::vector<cv::Vec3f> &rvecs,
        const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors,
        const std::array<std::vector<cv::Mat>, N_CAMERAS> &masks);

    std::vector<cv::Vec3f> random_normal(
        cv::Vec3f, cv::Vec3f, int rows, int cols, std::string bounds_for = "");

    std::vector<cv::Vec3f> random_uniform(cv::Vec3f lower_bound,
                                          cv::Vec3f upper_bound,
                                          int rows,
                                          int cols);

    std::vector<cv::Vec3f> sample_random_so3_rotvecs(int number_of_particles);

    cv::Vec3f power(cv::Vec3f, float);

    cv::Vec3f mean(const std::vector<cv::Vec3f> &);

    cv::Vec3f var(const std::vector<cv::Vec3f> &);

    cv::Mat _get_face_normals_cost(
        const std::vector<cv::Affine3f> &object_pose_matrices,
        const std::array<std::vector<FaceColor>, N_CAMERAS> &dominant_colors);

    cv::Mat _cost_of_out_of_bounds_projection(
        const std::array<cv::Mat, N_CAMERAS> &projected_points);

    void initialise_pos_cams_w_frame();
};

}  // namespace trifinger_object_tracking
