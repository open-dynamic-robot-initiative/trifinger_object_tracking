#include <trifinger_object_tracking/cube_visualizer.hpp>

namespace trifinger_object_tracking
{
std::array<cv::Mat, CubeVisualizer::N_CAMERAS> CubeVisualizer::draw_cube(
    const std::array<cv::Mat, CubeVisualizer::N_CAMERAS> &images,
    const ObjectPose &object_pose,
    bool fill_faces)
{
    std::array<cv::Mat, CubeVisualizer::N_CAMERAS> out_images;

    cv::Vec3d position, rotvec;
    cv::eigen2cv(object_pose.position, position);

    // convert quaternion to rotvec
    double theta = 2 * std::acos(object_pose.orientation[3]);
    Eigen::Vector3d eigen_rotvec;
    if (theta == 0)
    {
        eigen_rotvec << 0, 0, 0;
    }
    else
    {
        eigen_rotvec =
            theta / std::sin(theta / 2) * object_pose.orientation.head<3>();
    }
    cv::eigen2cv(eigen_rotvec, rotvec);

    Pose pose(position, rotvec, object_pose.confidence);

    pose_detector_.set_pose(pose);

    auto projected_cube_corners = pose_detector_.get_projected_points();
    for (size_t i = 0; i < N_CAMERAS; i++)
    {
        out_images[i] = images[i].clone();

        std::vector<cv::Point2f> imgpoints = projected_cube_corners[i];

        if (fill_faces)
        {
            for (auto [color, corner_indices] :
                 pose_detector_.get_visible_faces(i))
            {
                auto rgb = cube_model_.get_rgb(color);

                std::vector<cv::Point> corners = {imgpoints[corner_indices[0]],
                                                  imgpoints[corner_indices[1]],
                                                  imgpoints[corner_indices[2]],
                                                  imgpoints[corner_indices[3]]};
                cv::fillConvexPoly(out_images[i],
                                   corners,
                                   cv::Scalar(rgb[2], rgb[1], rgb[0], 0.7));
            }
        }
        else
        {
            // draw all the cube edges in the out_images[i]
            for (auto &it : cube_model_.edges)
            {
                cv::line(out_images[i],
                         imgpoints[it.second.c1],
                         imgpoints[it.second.c2],
                         cv::Scalar(100, 50, 0),
                         2);
            }

            // over-draw with the visible edges in a brighter colour
            for (auto [color, corner_indices] :
                 pose_detector_.get_visible_faces(i))
            {
                (void)color;  // suppress unused warning

                for (size_t ci = 0; ci < 4; ci++)
                {
                    cv::line(out_images[i],
                             imgpoints[corner_indices[ci]],
                             imgpoints[corner_indices[(ci + 1) % 4]],
                             cv::Scalar(255, 100, 0),
                             2);
                }
            }
        }
    }

    return out_images;
}
}  // namespace trifinger_object_tracking
