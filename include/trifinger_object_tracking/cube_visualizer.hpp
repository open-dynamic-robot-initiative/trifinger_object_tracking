/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <trifinger_cameras/parse_yml.h>

#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/object_pose.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/utils.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Visualise object pose in the camera images.
 *
 * The @ref draw_cube() method can be used to project the object into the camera
 * images and draw it either as wireframe or with filled faces for
 * visualisation/debugging purposes.
 */
class CubeVisualizer
{
public:
    static constexpr unsigned int N_CAMERAS = 3;

    /**
     * @param cube_model  Model of the object that is visualised.
     * @param camera_params  Camera calibration parameters.
     */
    CubeVisualizer(BaseCuboidModel::ConstPtr cube_model,
                   const std::array<trifinger_cameras::CameraParameters,
                                    N_CAMERAS> &camera_params)
        : cube_model_(cube_model), pose_detector_(cube_model, camera_params)
    {
    }

    /**
     * @param cube_model  Model of the object that is visualised.
     * @param camera_calib_files  Paths to the camera parameter files.
     */
    CubeVisualizer(BaseCuboidModel::ConstPtr cube_model,
                   const std::array<std::string, N_CAMERAS> &camera_calib_files)
        : CubeVisualizer(cube_model, load_camera_parameters(camera_calib_files))
    {
    }

    /**
     * @brief Draw the object into the images at the specified pose.
     *
     * @param images  Images from the cameras.  The images need to be ordered in
     *  the same way as the camera calibration parameters that are passed to the
     *  constructor.
     * @param object_pose  Pose of the object.
     * @param fill_faces  If false (default), only a monochrome wireframe of the
     *  object is drawn.  If true, the faces of the cuboid are filled with the
     *  corresponding colours.  The latter allows to verify the orientation
     *  (i.e. which face is pointing into which direction) but occludes more of
     *  the image.
     *
     * @return Images with the object visualisation.
     */
    std::array<cv::Mat, N_CAMERAS> draw_cube(
        const std::array<cv::Mat, N_CAMERAS> &images,
        const ObjectPose &object_pose,
        bool fill_faces = false);

private:
    BaseCuboidModel::ConstPtr cube_model_;
    PoseDetector pose_detector_;
};
}  // namespace trifinger_object_tracking
