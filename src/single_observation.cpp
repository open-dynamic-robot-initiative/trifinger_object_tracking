/**
 * @brief Run pose detection on image files of one camera observation.
 *
 * Load images of the three cameras from files "camera{60,180,300}.png", run the
 * pose detection on them and visualize the result.
 */
#include <iostream>

#include <ros/package.h>

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/cv_sub_images.hpp>
#include <trifinger_object_tracking/line_detector.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>

/**
 * @brief Load images camera{60,180,300}.png from the given directory.
 *
 * @param directory Directory containing the image files.
 *
 * @return Array with images [camera60.png, camera180.png, camera300.png]
 */
std::array<cv::Mat, 3> load_images(const std::string &directory)
{
    cv::Mat image60 = cv::imread(directory + "/camera60.png");
    cv::Mat image180 = cv::imread(directory + "/camera180.png");
    cv::Mat image300 = cv::imread(directory + "/camera300.png");

    if (image60.empty() || image180.empty() || image300.empty())
    {
        throw std::runtime_error("Failed to load image.");
    }

    return {image60, image180, image300};
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        std::cout << "Invalid number of arguments." << std::endl;
        std::cout << "Usage: " << argv[0]
                  << " image_directory [segmentation_model_directory]"
                  << std::endl;
        return 1;
    }
    const std::string data_dir = argv[1];

    // if no path is provided, load the segmentation model from
    // trifinger_object_tracking/data.
    std::string model_directory;
    if (argc < 3)
    {
        model_directory =
            ros::package::getPath("trifinger_object_tracking") + "/data";
    }
    else
    {
        model_directory = argv[2];
    }

    auto frames = load_images(data_dir);

    trifinger_object_tracking::CvSubImages subplot(cv::Size(frames[0].cols, frames[0].rows), 3, 5);
    cv::namedWindow("debug", cv::WINDOW_NORMAL);
    cv::resizeWindow(
        "debug", subplot.get_image().cols, subplot.get_image().rows);

    trifinger_object_tracking::CubeModel cube_model;
    trifinger_object_tracking::LineDetector line_detector(cube_model,
                                                          model_directory);
    trifinger_object_tracking::PoseDetector pose(cube_model);

    std::array<std::map<trifinger_object_tracking::ColorPair,
                        trifinger_object_tracking::Line>,
               3>
        lines;

    int i = 0;
    for (const cv::Mat &image : frames)
    {
        // FIXME: move this processing to somewhere else!
        cv::fastNlMeansDenoisingColored(image, image, 10, 10, 7, 21);
        cv::GaussianBlur(image, image, cv::Size(5, 5), 0);

        // TODO clone needed?
        lines[i] = line_detector.detect_lines(image.clone());

        subplot.set_subimg(line_detector.get_image(), i, 0);
        subplot.set_subimg(line_detector.get_segmented_image(), i, 1);
        subplot.set_subimg(
            line_detector.get_segmented_image_wout_outliers(), i, 2);
        subplot.set_subimg(line_detector.get_image_lines(), i, 3);

        i++;
    }

    pose.find_pose(lines);

    // visualize the detected pose
    auto projected_cube_corners = pose.get_projected_points();
    for (int i = 0; i < frames.size(); i++)
    {
        std::vector<cv::Point2f> imgpoints = projected_cube_corners[i];
        cv::Mat poseimg = frames[i].clone();
        // draw the cube edges in the image
        for (auto &it : cube_model.object_model_)
        {
            cv::Point p1, p2;
            p1.x = imgpoints[it.second.first].x;
            p1.y = imgpoints[it.second.first].y;
            p2.x = imgpoints[it.second.second].x;
            p2.y = imgpoints[it.second.second].y;

            cv::line(poseimg, p1, p2, cv::Scalar(255, 100, 0), 2);
        }
        subplot.set_subimg(poseimg, i, 4);
    }

    cv::Mat debug_img = subplot.get_image();

    // scale down debug image by factor 2, otherwise it is too big
    cv::Mat rescaled_debug_img;
    cv::resize(debug_img,
               rescaled_debug_img,
               cv::Size(debug_img.cols / 2, debug_img.rows / 2));
    cv::imwrite(data_dir.substr(data_dir.find_last_of("/\\")+1, 4)+".jpg", rescaled_debug_img);
//    cv::imshow("debug", rescaled_debug_img);
//    cv::waitKey(0);

    return 0;
}

