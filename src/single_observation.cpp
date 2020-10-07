/**
 * @brief Run pose detection on image files of one camera observation.
 *
 * Load images of the three cameras from files "camera{60,180,300}.png", run the
 * pose detection on them and visualize the result.
 */
#include <ros/package.h>
#include <trifinger_cameras/parse_yml.h>

#include <iostream>
#include <trifinger_object_tracking/cube_detector.hpp>

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

    std::array<trifinger_cameras::CameraParameters, 3> camera_params;
    // FIXME: This is a bit of a hack but for now just expect calibration files
    // with fixed names on level above data_dir
    std::string camera_name;
    bool success;
    success = trifinger_cameras::readCalibrationYml(
        data_dir + "/../camera_calib_60.yml", camera_name, camera_params[0]);
    if (!success || camera_name != "camera60")
    {
        throw std::runtime_error("Failed to load parameters of camera60 ");
    }

    success = trifinger_cameras::readCalibrationYml(
        data_dir + "/../camera_calib_180.yml", camera_name, camera_params[1]);
    if (!success || camera_name != "camera180")
    {
        throw std::runtime_error("Failed to load parameters of camera60");
    }

    success = trifinger_cameras::readCalibrationYml(
        data_dir + "/../camera_calib_300.yml", camera_name, camera_params[2]);
    if (!success || camera_name != "camera300")
    {
        throw std::runtime_error("Failed to load parameters of camera60");
    }

    trifinger_object_tracking::CubeDetector cube_detector(model_directory,
                                                          camera_params);

    trifinger_object_tracking::Pose pose = cube_detector.detect_cube(frames);

    std::cout << "Cube Position: " << pose.translation << std::endl;

#ifdef VISUALIZE
    cv::Mat debug_img = cube_detector.create_debug_image(false);

    // scale down debug image by factor 2, otherwise it is too big
    cv::Mat rescaled_debug_img;
    cv::resize(debug_img,
               rescaled_debug_img,
               cv::Size(debug_img.cols / 2, debug_img.rows / 2));

    time_t t = time(0);  // get time now
    struct tm *now = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%s", now);

    cv::imwrite("./temp_results/" +
                    data_dir.substr(data_dir.find_last_of("/\\") + 1, 4) +
                    "__" + std::string(buffer) + ".jpg",
                rescaled_debug_img);

    // cv::namedWindow("debug", cv::WINDOW_NORMAL);
    // cv::resizeWindow("debug", rescaled_debug_img.cols,
    // rescaled_debug_img.rows); cv::imshow("debug", rescaled_debug_img);
    // cv::waitKey(0);
#endif

    return 0;
}
