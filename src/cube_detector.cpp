#include <trifinger_object_tracking/cube_detector.hpp>

#include <thread>

namespace trifinger_object_tracking
{
CubeDetector::CubeDetector(const std::string &segmentation_model_dir,
                           const std::array<trifinger_cameras::CameraParameters,
                                            N_CAMERAS> &camera_params)
    : line_detectors_{LineDetector(cube_model_, segmentation_model_dir),
                      LineDetector(cube_model_, segmentation_model_dir),
                      LineDetector(cube_model_, segmentation_model_dir)},
      pose_detector_(cube_model_, camera_params)
{
}

Pose CubeDetector::detect_cube(const std::array<cv::Mat, N_CAMERAS> &images)
{
    ScopedTimer timer("CubeDetector/detect_cube");

    std::array<ColorEdgeLineList, N_CAMERAS> lines;
    std::array<std::vector<FaceColor>, N_CAMERAS> dominant_colors;
    std::array<std::vector<cv::Mat>, N_CAMERAS> masks;

    // run line detection multi-threaded (one thread per image)
    std::array<std::thread, N_CAMERAS> threads;
    for (int i = 0; i < N_CAMERAS; i++)
    {
        threads[i] = std::thread(
            [this, &lines, &dominant_colors, &masks](int i,
                                                     const cv::Mat &image) {
                lines[i] = line_detectors_[i].detect_lines(image);

                dominant_colors[i] = line_detectors_[i].get_dominant_colors();
                for (FaceColor color : dominant_colors[i])
                {
                    masks[i].push_back(line_detectors_[i].get_mask(color));
                }
            },
            i,
            images[i]);
    }
    for (std::thread &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return pose_detector_.find_pose(lines, dominant_colors, masks);
}

cv::Mat CubeDetector::create_debug_image(bool fill_faces) const
{
    cv::Mat image0 = line_detectors_[0].get_image();
    trifinger_object_tracking::CvSubImages subplot(
        cv::Size(image0.cols, image0.rows), 3, 5);

    auto projected_cube_corners = pose_detector_.get_projected_points();
    for (int i = 0; i < N_CAMERAS; i++)
    {
        cv::Mat image = line_detectors_[i].get_image();
        subplot.set_subimg(image, i, 0);
        subplot.set_subimg(line_detectors_[i].get_segmented_image(), i, 1);
        subplot.set_subimg(line_detectors_[i].get_front_line_image(), i, 2);
        subplot.set_subimg(line_detectors_[i].get_image_lines(), i, 3);

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
                cv::fillConvexPoly(
                    image, corners, cv::Scalar(rgb[2], rgb[1], rgb[0], 0.7));
            }
        }
        else
        {
            // draw all the cube edges in the image
            for (auto &it : cube_model_.edges)
            {
                cv::line(image,
                         imgpoints[it.second.c1],
                         imgpoints[it.second.c2],
                         cv::Scalar(100, 50, 0),
                         2);
            }

            // over-draw with the visible edges in a brighter colour
            for (auto [color, corner_indices] :
                 pose_detector_.get_visible_faces(i))
            {
                auto rgb = cube_model_.get_rgb(color);

                for (size_t ci = 0; ci < 4; ci++)
                {
                    cv::line(image,
                             imgpoints[corner_indices[ci]],
                             imgpoints[corner_indices[(ci + 1) % 4]],
                             cv::Scalar(255, 100, 0),
                             2);
                }
            }
        }

        subplot.set_subimg(image, i, 4);
    }

    return subplot.get_image();
}

}  // namespace trifinger_object_tracking

