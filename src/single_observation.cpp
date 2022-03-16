/**
 * @brief Run pose detection on image files of one camera observation.
 *
 * Load images of the three cameras from files "camera{60,180,300}.png", run the
 * pose detection on them and visualize the result.
 */
#include <trifinger_cameras/parse_yml.h>

#include <iostream>
#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/program_options.hpp>
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

class Args : public trifinger_object_tracking::ProgramOptions
{
public:
    std::string data_dir, cube_model_name, debug_out_dir;

    std::string help() const override
    {
        return R"(Run pose detection on image files of one camera observation.

Load images of the three cameras from files "camera{60,180,300}.png", run the
pose detection on them and visualize the result.

Usage:  single_observation [options] <data-dir>

)";
    }

    void add_options(boost::program_options::options_description &options,
                     boost::program_options::positional_options_description
                         &positional) override
    {
        namespace po = boost::program_options;
        // clang-format off
        options.add_options()
            ("data-dir,d",
             po::value<std::string>(&data_dir)->required(),
             "Directory in which the images are stored.")
            ("object-model,o",
             po::value<std::string>(&cube_model_name)->required(),
             "Name of the object model (e.g. 'CubeV2').")
            ("debug-out",
             po::value<std::string>(&debug_out_dir),
             "If set, write debug files to this directory.")
            ;
        // clang-format on

        positional.add("data-dir", 1);
    }
};

int main(int argc, char **argv)
{
    // read command line arguments
    Args args;
    if (!args.parse_args(argc, argv))
    {
        return 1;
    }

    auto frames = load_images(args.data_dir);

    // FIXME: This is a bit of a hack but for now just expect calibration files
    // with fixed names on level above data_dir
    std::array<trifinger_cameras::CameraParameters, 3> camera_params =
        trifinger_object_tracking::load_camera_parameters({
            args.data_dir + "/../camera_calib_60.yml",
            args.data_dir + "/../camera_calib_180.yml",
            args.data_dir + "/../camera_calib_300.yml",
        });

    auto cube_model =
        trifinger_object_tracking::get_model_by_name(args.cube_model_name);
    trifinger_object_tracking::CubeDetector cube_detector(cube_model,
                                                          camera_params);

    trifinger_object_tracking::ObjectPose pose =
        cube_detector.detect_cube_single_thread(frames);

    std::cout << "Cube Position: " << pose.position << std::endl;

#ifdef VISUALIZE
    cv::Mat debug_img = cube_detector.create_debug_image(false);

    if (args.debug_out_dir.empty())
    {
        cv::namedWindow("debug", cv::WINDOW_NORMAL);
        cv::resizeWindow("debug", debug_img.cols, debug_img.rows);
        cv::imshow("debug", debug_img);
        cv::waitKey(0);
    }
    else
    {
        time_t now = time(0);  // get time now
        std::string out_file =
            args.debug_out_dir + "/" +
            args.data_dir.substr(args.data_dir.find_last_of("/\\") + 1, 4) +
            "__" + std::to_string(now) + ".jpg";
        std::cout << "Write to " << out_file << std::endl;
        cv::imwrite(out_file, debug_img);
    }
#endif

    return 0;
}
