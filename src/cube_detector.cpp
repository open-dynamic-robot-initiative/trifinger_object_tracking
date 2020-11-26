#include <trifinger_object_tracking/cube_detector.hpp>

#include <opencv2/core/eigen.hpp>
#include <thread>
#include <trifinger_object_tracking/utils.hpp>

namespace trifinger_object_tracking
{
CubeDetector::CubeDetector(const std::array<trifinger_cameras::CameraParameters,
                                            N_CAMERAS> &camera_params)
    : color_segmenters_{ColorSegmenter(cube_model_),
                        ColorSegmenter(cube_model_),
                        ColorSegmenter(cube_model_)},
      pose_detector_(cube_model_, camera_params)
{
}

CubeDetector::CubeDetector(
    const std::array<std::string, N_CAMERAS> &camera_param_files)
    : CubeDetector(
          trifinger_object_tracking::load_camera_parameters(camera_param_files))
{
}

ObjectPose CubeDetector::detect_cube(const std::array<cv::Mat, N_CAMERAS> &images)
{
    // ScopedTimer timer("CubeDetector/detect_cube");

    std::array<std::vector<FaceColor>, N_CAMERAS> dominant_colors;
    std::array<std::vector<cv::Mat>, N_CAMERAS> masks;

    // run line detection multi-threaded (one thread per image)
    std::array<std::thread, N_CAMERAS> threads;
    for (int i = 0; i < N_CAMERAS; i++)
    {
        threads[i] = std::thread(
            [this, &dominant_colors, &masks](int i, const cv::Mat &image) {
                color_segmenters_[i].detect_colors(image);

                dominant_colors[i] = color_segmenters_[i].get_dominant_colors();
                for (FaceColor color : dominant_colors[i])
                {
                    masks[i].push_back(color_segmenters_[i].get_mask(color));
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

    Pose pose = pose_detector_.find_pose(dominant_colors, masks);
    return convert_pose(pose);
}

ObjectPose CubeDetector::detect_cube_single_thread(
    const std::array<cv::Mat, N_CAMERAS> &images)
{
    // ScopedTimer timer("CubeDetector/detect_cube");

    std::array<std::vector<FaceColor>, N_CAMERAS> dominant_colors;
    std::array<std::vector<cv::Mat>, N_CAMERAS> masks;

    for (int i = 0; i < N_CAMERAS; i++)
    {
        color_segmenters_[i].detect_colors(images[i]);

        dominant_colors[i] = color_segmenters_[i].get_dominant_colors();
        for (FaceColor color : dominant_colors[i])
        {
            masks[i].push_back(color_segmenters_[i].get_mask(color));
        }
    }

    Pose pose = pose_detector_.find_pose(dominant_colors, masks);
    return convert_pose(pose);
}

cv::Mat CubeDetector::create_debug_image(bool fill_faces) const
{
    cv::Mat image0 = color_segmenters_[0].get_image();
    trifinger_object_tracking::CvSubImages subplot(
        cv::Size(image0.cols, image0.rows), 3, 3);

    auto projected_cube_corners = pose_detector_.get_projected_points();
    for (int i = 0; i < N_CAMERAS; i++)
    {
        cv::Mat image = color_segmenters_[i].get_image();
        subplot.set_subimg(image, i, 0);
        subplot.set_subimg(color_segmenters_[i].get_segmented_image(), i, 1);

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

        subplot.set_subimg(image, i, 2);
    }

    cv::Mat complete_image = subplot.get_image();
    cv::Mat complete_image_with_text_field =
        cv::Mat::ones(100, complete_image.cols, complete_image.type());

    complete_image_with_text_field.push_back(complete_image);

    std::string text_misclassied_pixels =
        "num_misclassified_pixels: " +
        std::to_string(pose_detector_.get_num_misclassified_pixels());
    std::string text_segmented_pixels =
        "segmented_pixels_ratio: " +
        std::to_string(pose_detector_.get_segmented_pixels_ratio());
    std::string text_confidence =
        "confidence: " + std::to_string(pose_detector_.get_confidence());

    cv::putText(complete_image_with_text_field,
                text_misclassied_pixels,
                cv::Point(20, 30),          // Coordinates
                cv::FONT_HERSHEY_PLAIN,     // Font
                1.5,                        // Scale. 2.0 = 2x bigger
                cv::Scalar(255, 255, 255),  // BGR Color
                2,                          // Line Thickness (Optional)
                cv::LINE_AA                 // Anti-alias (Optional)
    );
    cv::putText(complete_image_with_text_field,
                text_segmented_pixels,
                cv::Point(20, 60),          // Coordinates
                cv::FONT_HERSHEY_PLAIN,     // Font
                1.5,                        // Scale. 2.0 = 2x bigger
                cv::Scalar(255, 255, 255),  // BGR Color
                2,                          // Line Thickness (Optional)
                cv::LINE_AA                 // Anti-alias (Optional)
    );
    cv::putText(complete_image_with_text_field,
                text_confidence,
                cv::Point(20, 90),          // Coordinates
                cv::FONT_HERSHEY_PLAIN,     // Font
                1.5,                        // Scale. 2.0 = 2x bigger
                cv::Scalar(255, 255, 255),  // BGR Color
                2,                          // Line Thickness (Optional)
                cv::LINE_AA                 // Anti-alias (Optional)
    );

    return complete_image_with_text_field;
}

ObjectPose CubeDetector::convert_pose(const Pose &pose)
{
    ObjectPose object_pose;

    // convert rotation vector to quaternion
    // www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
    //
    //     qx = ax * sin(angle/2)
    //     qy = ay * sin(angle/2)
    //     qz = az * sin(angle/2)
    //     qw = cos(angle/2)
    //
    float angle = cv::norm(pose.rotation);
    cv::Vec3f qxyz = pose.rotation * std::sin(angle / 2) / angle;
    cv::Vec4d quaternion(qxyz[0], qxyz[1], qxyz[2], std::cos(angle / 2));

    cv::cv2eigen(static_cast<cv::Vec3d>(pose.translation),
                 object_pose.position);
    cv::cv2eigen(quaternion, object_pose.orientation);
    object_pose.confidence = pose.confidence;

    return object_pose;
}

CubeDetector create_trifingerpro_cube_detector()
{
    return CubeDetector({
        "/etc/trifingerpro/camera60_cropped_and_downsampled.yml",
        "/etc/trifingerpro/camera180_cropped_and_downsampled.yml",
        "/etc/trifingerpro/camera300_cropped_and_downsampled.yml",
    });
}

}  // namespace trifinger_object_tracking
