/**
 * @brief Run pose detection on image files of one camera observation.
 *
 * Load images of the three cameras from files "camera{60,180,300}.png", run the
 * pose detection on them and visualize the result.
 */
#include <trifinger_cameras/parse_yml.h>

#include <iostream>
#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/utils.hpp>

#define VISUALIZE

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
                  << " image_directory [output_directory]" << std::endl;
        return 1;
    }
    const std::string data_dir = argv[1];
    const std::string debug_out_dir = argc > 2 ? argv[2] : "";

    auto frames = load_images(data_dir);

    // FIXME: This is a bit of a hack but for now just expect calibration files
    // with fixed names on level above data_dir
    std::array<trifinger_cameras::CameraParameters, 3> camera_params =
        trifinger_object_tracking::load_camera_parameters({
            data_dir + "/../camera_calib_60.yml",
            data_dir + "/../camera_calib_180.yml",
            data_dir + "/../camera_calib_300.yml",
        });

    trifinger_object_tracking::CubeDetector cube_detector(camera_params);

    trifinger_object_tracking::ObjectPose pose =
        cube_detector.detect_cube_single_thread(frames);

    std::cout << "Cube Position: " << pose.position << std::endl;

#ifdef VISUALIZE
    cv::Mat debug_img = cube_detector.create_debug_image(false);

    time_t t = time(0);  // get time now
    struct tm *now = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%s", now);

    if (debug_out_dir.empty())
    {
        cv::namedWindow("debug", cv::WINDOW_NORMAL);
        cv::resizeWindow("debug", debug_img.cols, debug_img.rows);
        cv::imshow("debug", debug_img);
        cv::waitKey(0);
    }
    else
    {
        std::string out_file =
            debug_out_dir + "/" +
            data_dir.substr(data_dir.find_last_of("/\\") + 1, 4) + "__" +
            std::string(buffer) + ".jpg";
        std::cout << "Write to " << out_file << std::endl;
        cv::imwrite(out_file, debug_img);
    }
#endif

    return 0;
}
