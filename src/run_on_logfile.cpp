/**
 * @file
 * @brief Run object tracker on a camera logfile.
 *
 * Expects as argument the path to the directory containing the following files:
 *
 * - camera_data.dat: Camera log file (TriCameraObjectObservation)
 * - camera60.yml: Calibration parameters of camera60
 * - camera180.yml: Calibration parameters of camera180
 * - camera300.yml: Calibration parameters of camera300
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <robot_interfaces/sensors/sensor_log_reader.hpp>
#include <string>

// TODO: not sure why but if "sensor_logger.hpp" is not included, the log reader
// will have a cereal-related build error.
#include <robot_interfaces/sensors/sensor_logger.hpp>
#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/tricamera_object_observation.hpp>
#include <trifinger_object_tracking/program_options.hpp>
#include <trifinger_object_tracking/utils.hpp>

constexpr unsigned N_CAMERAS = 3;

bool open_video_writer(cv::VideoWriter &writer,
                       const std::string &filename,
                       int fourcc,
                       double fps,
                       cv::Size frame_size)
{
    writer.open(filename, fourcc, fps, frame_size);
    if (!writer.isOpened())
    {
        std::cerr << "Could not open the output video for write: " << filename
                  << std::endl;
        return false;
    }

    return true;
}


class Args : public trifinger_object_tracking::ProgramOptions
{
public:
    std::string data_dir, debug_video_file, cube_model_name;

    std::string help() const override
    {
        return R"HELP(Run object tracker on a camera logfile.
 
Expects as argument <data-dir> the path to the directory containing the
following files:

- camera_data.dat: Camera log file (TriCameraObjectObservation)
- camera60.yml: Calibration parameters of camera60
- camera180.yml: Calibration parameters of camera180
- camera300.yml: Calibration parameters of camera300

Usage:  run_on_logfile [options] <data-dir>

)HELP";
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
             "Directory in which the logfiles are stored.")
            ("object-model,o",
             po::value<std::string>(&cube_model_name)->required(),
             "Name of the object model (e.g. 'CubeV2').")
            ("save-video",
             po::value<std::string>(&debug_video_file),
             "Save debug video to the specified path.")
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

    std::array<trifinger_cameras::CameraParameters, 3> camera_params =
        trifinger_object_tracking::load_camera_parameters({
            args.data_dir + "/camera60.yml",
            args.data_dir + "/camera180.yml",
            args.data_dir + "/camera300.yml",
        });

    auto cube_model =
        trifinger_object_tracking::get_model_by_name(args.cube_model_name);
    trifinger_object_tracking::CubeDetector cube_detector(cube_model,
                                                          camera_params);

    robot_interfaces::SensorLogReader<
        trifinger_object_tracking::TriCameraObjectObservation>
        log_reader(args.data_dir + "/camera_data.dat");

    cv::VideoWriter debug_video;
    cv::namedWindow("Object Tracking", cv::WINDOW_NORMAL);
    bool first_iteration = true;
    for (const auto &observation : log_reader.data)
    {
        std::array<cv::Mat, N_CAMERAS> images_bgr;
        for (size_t i = 0; i < N_CAMERAS; i++)
        {
            cv::cvtColor(observation.cameras[i].image,
                         images_bgr[i],
                         cv::COLOR_BayerBG2BGR);
        }

        cube_detector.detect_cube_single_thread(images_bgr);

        cv::Mat debug_img = cube_detector.create_debug_image(false);

        // in the first iteration, resize the window to fit the image
        if (first_iteration)
        {
            first_iteration = false;
            cv::resizeWindow("Object Tracking", debug_img.cols, debug_img.rows);

            if (!args.debug_video_file.empty())
            {
                constexpr double FPS = 10.0;
                bool ok = open_video_writer(
                    debug_video,
                    args.debug_video_file,
                    cv::VideoWriter::fourcc('X', 'V', 'I', 'D'),
                    FPS,
                    debug_img.size());
                if (!ok)
                {
                    return 1;
                }
            }
        }

        if (!args.debug_video_file.empty())
        {
            debug_video.write(debug_img);
        }

        cv::imshow("Object Tracking", debug_img);
        char key = cv::waitKey(3);
        if (key == 'q')
        {
            break;
        }
    }

    return 0;
}
