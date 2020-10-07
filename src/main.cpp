#include <chrono>
#include <fstream>
#include <ostream>
#include <thread>

#include <trifinger_cameras/parse_yml.h>

#include <trifinger_object_tracking/utils.hpp>
#include <trifinger_object_tracking/cube_detector.hpp>

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

    trifinger_object_tracking::CubeDetector cube_detector("../data",
                                                          camera_params);


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

        cube_detector.detect_cube({image60, image180, image300});

        std::cout << "Pose detected\n";

        cv::Mat debug_img = cube_detector.create_debug_image();

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
