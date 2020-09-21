#include <chrono>
#include <fstream>
#include <ostream>
#include <thread>

#include <trifinger_cameras/parse_yml.h>

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/cv_sub_images.hpp>
#include <trifinger_object_tracking/line_detector.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/utils.hpp>

int debug = 1;
int cols_plot = 5;

using namespace trifinger_object_tracking;

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
    cv::VideoCapture cap_60("../data/videos_trifingerone/11/video_60.avi");
    cv::VideoCapture cap_180("../data/videos_trifingerone/11/video_180.avi");
    cv::VideoCapture cap_300("../data/videos_trifingerone/11/video_300.avi");

    // initialize objects
    trifinger_object_tracking::CubeModel cube_model;
    trifinger_object_tracking::LineDetector line_detector(cube_model,
                                                          "../data");
    std::array<trifinger_cameras::CameraParameters, 3> camera_params;
    std::string camera_name;
    bool success;
    success = trifinger_cameras::readCalibrationYml(
        "../data/videos_trifingerone/camera_calib_60.yml",
        camera_name,
        camera_params[0]);
    assert(success && camera_name == "camera60");
    success = trifinger_cameras::readCalibrationYml(
        "../data/videos_trifingerone/camera_calib_180.yml",
        camera_name,
        camera_params[1]);
    assert(success && camera_name == "camera180");
    success = trifinger_cameras::readCalibrationYml(
        "../data/videos_trifingerone/camera_calib_300.yml",
        camera_name,
        camera_params[2]);
    assert(success && camera_name == "camera300");

    trifinger_object_tracking::PoseDetector pose(cube_model, camera_params);
    // start plot
    trifinger_object_tracking::CvSubImages subplot(cv::Size(720, 540), 3, 5);
    cv::namedWindow("debug", cv::WINDOW_NORMAL);
    cv::resizeWindow(
        "debug", subplot.get_image().cols, subplot.get_image().rows);
    // end plot

    // for saving to a video file
    int frame_width = 1815;
    int frame_height = 820;
    cv::Size frame_size(frame_width, frame_height);
    int frames_per_second = 10;
    cv::VideoWriter oVideoWriter("./video_final_2.avi",
                                 cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                 frames_per_second,
                                 frame_size,
                                 true);

    // If the VideoWriter object is not initialized successfully, exit the
    // program
    if (oVideoWriter.isOpened() == false)
    {
        std::cout << "Cannot save the video to a file" << std::endl;
        std::cin.get();  // wait for any key press
        return -1;
    }

    int current_frame = 300;
    cv::Mat image60;
    cv::Mat image180;
    cv::Mat image300;
    while (current_frame < 350)
    {
        current_frame++;
        std::cout << "processing frame: " << current_frame << std::endl;
        cap_60.set(cv::CAP_PROP_POS_FRAMES, (current_frame + 1) * 10);
        cap_180.set(cv::CAP_PROP_POS_FRAMES, (current_frame + 1) * 10);
        cap_300.set(cv::CAP_PROP_POS_FRAMES, (current_frame + 1) * 10);
        cap_60.read(image60);
        cap_180.read(image180);
        cap_300.read(image300);
        auto frames = {image60, image180, image300};
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
            subplot.set_subimg(line_detector.get_front_line_image(), i, 2);
            subplot.set_subimg(line_detector.get_image_lines(), i, 3);

            i++;
        }

        pose.find_pose(lines);

        std::cout << "Pose detected\n";

        if (debug == 1)
        {
            auto projected_cube_corners = pose.get_projected_points();
            cv::Mat poseimg = image60.clone();
            std::vector<cv::Point2f> imgpoints = projected_cube_corners[0];
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
            subplot.set_subimg(poseimg, 0, 4);
            poseimg = image180.clone();
            imgpoints = projected_cube_corners[1];
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
            subplot.set_subimg(poseimg, 1, 4);
            poseimg = image300.clone();
            imgpoints = projected_cube_corners[2];
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
            subplot.set_subimg(poseimg, 2, 4);
        }

        cv::Mat debug_img = subplot.get_image();

        // scale down debug image by factor 2, otherwise it is too big
        cv::Mat rescaled_debug_img;
        cv::resize(debug_img,
                   rescaled_debug_img,
                   cv::Size(debug_img.cols / 2, debug_img.rows / 2));

        // write the video frame to the file
        oVideoWriter.write(rescaled_debug_img);

        int show = 0;
        if (show == 1)
        {
            cv::imshow("debug", rescaled_debug_img);
            char key = cv::waitKey(0);

            if (key == 'q')
            {
                break;
            }
        }
    }
    // Flush and close the video file
    oVideoWriter.release();
    return 0;
}
