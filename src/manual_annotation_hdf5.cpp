/**
 * @file
 * @brief Manual find ground truth pose for a TriCamera HDF5 file.
 */
#include <cmath>
#include <filesystem>
#include <fstream>

#include <fmt/format.h>
#include <Eigen/Eigen>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/hdf.hpp>
#include <opencv2/videoio.hpp>

#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/cube_visualizer.hpp>

constexpr int N_CAMERAS = 3;

void zoom_to_object(std::array<cv::Mat, N_CAMERAS> &images,
                    const std::vector<std::vector<cv::Point2f>> &object_points)
{
    constexpr int padding = 15;

    for (size_t i = 0; i < N_CAMERAS; ++i)
    {
        cv::Rect boundingbox = cv::boundingRect(object_points[i]);
        boundingbox.x -= padding;
        boundingbox.y -= padding;
        boundingbox.height += 2 * padding;
        boundingbox.width += 2 * padding;

        cv::Size orig_size = images[i].size();

        // cut out the bounding box area with the given padding
        images[i] = images[i](boundingbox);

        // scale back to original size
        cv::resize(images[i], images[i], orig_size);
    }
}

int main(int argc, char *argv[])
{
    const cv::String keys =
        "{@camera_info_60 | <none> | Camera parameter file for camera60 }"
        "{@camera_info_180 | <none> | Camera parameter file for camera180 }"
        "{@camera_info_300 | <none> | Camera parameter file for camera300 }"
        "{@object_model | <none> | Name of the object model to use }"
        "{@hdf5_file | <none> | HDF5 file with TriCamera data }"
        "{frame | 0 | Frame number }"
        "{help h | | print this message }";
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about(
        "Manual find ground truth object pose for a TriCamera HDF5 file");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    cv::String camera_info_60 = parser.get<cv::String>(0);
    cv::String camera_info_180 = parser.get<cv::String>(1);
    cv::String camera_info_300 = parser.get<cv::String>(2);
    cv::String object_model_name = parser.get<cv::String>(3);
    cv::String hdf5_file = parser.get<cv::String>(4);
    int frame_number = parser.get<int>("frame");

    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    if (!std::filesystem::exists(hdf5_file))
    {
        fmt::print(stderr, "File {} does not exist\n", hdf5_file);
        return 1;
    }

    cv::Ptr<cv::hdf::HDF5> h5io = cv::hdf::open(hdf5_file);

    // validate the file
    cv::String attr_magic_name = "magic";
    int magic_byte = 0;
    if (!h5io->atexists(attr_magic_name))
    {
        fmt::print(
            stderr,
            "Input file doesn't seem to be a TriCamera log file (no magic "
            "byte)\n");
        return 1;
    }
    h5io->atread(&magic_byte, attr_magic_name);
    if (magic_byte != 0x3CDA7A00)
    {
        fmt::print(
            stderr,
            "Input file doesn't seem to be a TriCamera log file (bad magic "
            "byte)\n");
        return 1;
    }

    cv::String attr_formatversion_name = "format_version";
    int format_version = 0;
    h5io->atread(&format_version, attr_formatversion_name);
    if (format_version != 1 and format_version != 2)
    {
        fmt::print(stderr,
                   "Unsupported file format version {}.  Supported versions "
                   "are 1-2.\n",
                   format_version);
        return 1;
    }

    auto object_model =
        trifinger_object_tracking::get_model_by_name(object_model_name);
    trifinger_object_tracking::CubeDetector cube_detector(object_model,
                                                          {
                                                              camera_info_60,
                                                              camera_info_180,
                                                              camera_info_300,
                                                          });
    trifinger_object_tracking::CubeVisualizer visualizer(object_model,
                                                         {
                                                             camera_info_60,
                                                             camera_info_180,
                                                             camera_info_300,
                                                         });
    visualizer.set_line_thickness(1);

    int image_width, image_height;
    h5io->atread(&image_width, "image_width");
    h5io->atread(&image_height, "image_height");

    int n_images = h5io->dsgetsize("images")[0];
    int n_views = h5io->dsgetsize("images")[1];

    if (n_views != N_CAMERAS)
    {
        fmt::print(stderr,
                   "Input file doesn't seem to be a TriCamera log file (wrong "
                   "number of views)\n");
        return 1;
    }

    if (frame_number < 0 || frame_number >= n_images)
    {
        fmt::print(stderr,
                   "Frame number must be between 0 and {} (inclusive)\n",
                   n_images - 1);
        return 1;
    }

    std::array<cv::Mat, N_CAMERAS> images;
    for (int j = 0; j < N_CAMERAS; j++)
    {
        cv::Mat image;

        // Set offset and counts to read only the image of frame i/camera j
        int offset[4] = {frame_number, j, 0, 0};
        int counts[4] = {1, 1, image_height, image_width};
        h5io->dsread(image, "images", offset, counts);

        // Image is now of shape (1, 1, height, width).  Reshape to get rid
        // of the first two dimensions.
        image = image.reshape(1, image_height);

        // Images in the HDF5 file are raw Bayer images.  Convert to BGR.
        cv::cvtColor(image, image, cv::COLOR_BayerBG2BGR);

        images[j] = image;
    }

    float angle120 = 2 * M_PI / 3;
    std::array<cv::Affine3d, 3> view_transforms{
        // camera60
        cv::Affine3d(cv::Affine3d::Mat3(std::cos(-angle120),
                                        -std::sin(-angle120),
                                        0,
                                        std::sin(-angle120),
                                        std::cos(-angle120),
                                        0,
                                        0,
                                        0,
                                        1)),
        // camera180
        cv::Affine3d::Identity(),
        // camera300
        cv::Affine3d(cv::Affine3d::Mat3(std::cos(angle120),
                                        -std::sin(angle120),
                                        0,
                                        std::sin(angle120),
                                        std::cos(angle120),
                                        0,
                                        0,
                                        0,
                                        1))};

    size_t active_view = 1;
    constexpr float position_step_size = 0.0005;
    constexpr float rotation_step_size = 0.01;
    bool zoom = false;
    bool fill_faces = true;

    // get initial pose estimate
    trifinger_object_tracking::ObjectPose pose =
        cube_detector.detect_cube_single_thread(images);

    // convert pose to cv::Affine3d
    Eigen::Matrix4d pose_matrix = pose.affine().matrix();
    cv::Mat pose_matrix_cv;
    cv::eigen2cv(pose_matrix, pose_matrix_cv);
    cv::Affine3d pose_cv(pose_matrix_cv);

    fmt::print(
        "=====================================\n"
        "Instructions:\n"
        "      q:  quit\n"
        "      z:  toggle zoom\n"
        "      c:  toggle face colour\n"
        "      x:  set pose to centre of arena\n"
        "  1/2/3:  select camera60/180/300\n"
        "    w/s:  move up/down\n"
        "    a/d:  move left/right\n"
        "    r/f:  move forward\n"
        "    i/k:  rotate up/down\n"
        "    j/l:  rotate left/right\n"
        "    u/o:  rotate around z-axis\n"
        "      p:  print current pose\n"
        "      g:  save current pose to ground\n"
        "          truth file\n"
        "=====================================\n");

    while (true)
    {
        // convert pose_cv back to ObjectPose for visualization
        cv::cv2eigen(pose_cv.matrix, pose_matrix);
        pose = trifinger_object_tracking::ObjectPose(pose_matrix);

        float alpha = fill_faces ? 0.4 : 0.9;
        std::array<cv::Mat, N_CAMERAS> visualization_images =
            visualizer.draw_cube(images, pose, fill_faces, alpha);

        if (zoom)
        {
            zoom_to_object(visualization_images,
                           visualizer.get_projected_points(pose));
        }

        // mark active view
        cv::line(visualization_images[active_view],
                 cv::Point(0, 0),
                 cv::Point(visualization_images[active_view].cols, 0),
                 cv::Scalar(0, 200, 0),
                 10);

        // combine all images horizontally into one output image
        cv::Mat viz;
        cv::hconcat(visualization_images, viz);

        cv::Affine3d transform = cv::Affine3d::Identity();

        cv::imshow("Estimated Pose", viz);
        int key = cv::waitKey(0);
        if (key == 'q')
        {
            break;
        }
        else if (key == 'z')
        {
            zoom = !zoom;
        }
        else if (key == 'c')
        {
            fill_faces = !fill_faces;
        }
        else if (key == 'x')
        {
            // TODO: the z value is specific for the 65mm cube, so not very
            // generic here...
            pose_cv.translation(cv::Vec3d(0, 0, 0.0325));
            pose_cv.rotation(cv::Vec3d(0, 0, 0));
        }
        else if (key == ' ')
        {
            pose_cv = cv::Affine3d::Identity();
        }
        else if (key == '1')
        {
            active_view = 0;
        }
        else if (key == '2')
        {
            active_view = 1;
        }
        else if (key == '3')
        {
            active_view = 2;
        }
        else if (key == 'w')
        {
            transform.translation(cv::Vec3d(0, position_step_size, 0));
        }
        else if (key == 's')
        {
            transform.translation(cv::Vec3d(0, -position_step_size, 0));
        }
        else if (key == 'a')
        {
            transform.translation(cv::Vec3d(-position_step_size, 0, 0));
        }
        else if (key == 'd')
        {
            transform.translation(cv::Vec3d(position_step_size, 0, 0));
        }
        else if (key == 'r')
        {
            transform.translation(cv::Vec3d(0, 0, position_step_size));
        }
        else if (key == 'f')
        {
            transform.translation(cv::Vec3d(0, 0, -position_step_size));
        }
        else if (key == 'i')
        {
            transform.rotation(cv::Vec3d(-rotation_step_size, 0, 0));
        }
        else if (key == 'k')
        {
            transform.rotation(cv::Vec3d(rotation_step_size, 0, 0));
        }
        else if (key == 'j')
        {
            transform.rotation(cv::Vec3d(0, -rotation_step_size, 0));
        }
        else if (key == 'l')
        {
            transform.rotation(cv::Vec3d(0, rotation_step_size, 0));
        }
        else if (key == 'u')
        {
            transform.rotation(cv::Vec3d(0, 0, rotation_step_size));
        }
        else if (key == 'o')
        {
            transform.rotation(cv::Vec3d(0, 0, -rotation_step_size));
        }
        else if (key == 'p')
        {
            std::cout << "Pose (matrix):\n"
                      << pose_cv.matrix << std::endl
                      << std::endl;

            std::cout << "{\"translation\": " << pose_cv.translation()
                      << ", \"rotation\": " << pose_cv.rvec() << "}"
                      << std::endl;
        }
        else if (key == 'g')
        {
            std::filesystem::path hdf5_path(hdf5_file);
            std::filesystem::path ground_truth_file(hdf5_path);
            ground_truth_file.replace_extension("ground_truth.json");

            std::ofstream ground_truth_stream(ground_truth_file);
            if (!ground_truth_stream.is_open())
            {
                fmt::print(
                    stderr, "Failed to open {}\n", ground_truth_file.string());
                return 1;
            }

            ground_truth_stream
                << "{\"pose\": " << cv::Mat(pose_cv.matrix).reshape(1, 1) << "}"
                << std::endl;
            ground_truth_stream.close();

            fmt::print("Ground truth saved to {}\n",
                       ground_truth_file.string());
        }

        // operation space is using the rotation of view_transforms and shifts
        // the origin to the center of the object pose (so the object rotates
        // around its centre)
        cv::Affine3d operation_space = view_transforms[active_view];
        operation_space.translation(
            -(view_transforms[active_view] * pose_cv).translation());

        // transform pose to space of active view, apply user transform and then
        // transform back to world space
        pose_cv = operation_space.inv() * transform * operation_space * pose_cv;
    }

    h5io->close();

    return 0;
}
