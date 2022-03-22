/**
 * @file
 * @brief Measure object detection performance using a logfile.
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
#include <string>

#include <real_time_tools/timer.hpp>
#include <robot_interfaces/sensors/sensor_log_reader.hpp>

// TODO: not sure why but if "sensor_logger.hpp" is not included, the log reader
// will have a cereal-related build error.
#include <robot_interfaces/sensors/sensor_logger.hpp>
#include <trifinger_object_tracking/cube_detector.hpp>
#include <trifinger_object_tracking/tricamera_object_observation.hpp>
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

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        std::cout << "Invalid number of arguments." << std::endl;
        std::cout << "Usage: " << argv[0] << " data_directory [--multithread]"
                  << std::endl;
        return 1;
    }
    const std::string data_dir = argv[1];
    const bool multithread =
        argc > 2 ? std::string(argv[2]) == "--multithread" : false;

    if (multithread)
    {
        std::cout << "Use multi-threaded object detection." << std::endl;
    }
    else
    {
        std::cout << "Use single-threaded object detection." << std::endl;
    }

    std::array<trifinger_cameras::CameraParameters, 3> camera_params =
        trifinger_object_tracking::load_camera_parameters({
            data_dir + "/camera60.yml",
            data_dir + "/camera180.yml",
            data_dir + "/camera300.yml",
        });

    trifinger_object_tracking::CubeDetector cube_detector(camera_params);

    robot_interfaces::SensorLogReader<
        trifinger_object_tracking::TriCameraObjectObservation>
        log_reader(data_dir + "/camera_data.dat");

    real_time_tools::Timer timer;
    for (const auto &observation : log_reader.data)
    {
        std::array<cv::Mat, N_CAMERAS> images_bgr;
        for (size_t i = 0; i < N_CAMERAS; i++)
        {
            cv::cvtColor(observation.cameras[i].image,
                         images_bgr[i],
                         cv::COLOR_BayerBG2BGR);
        }

        if (multithread)
        {
            timer.tic();
            cube_detector.detect_cube(images_bgr);
            timer.tac();
        }
        else
        {
            timer.tic();
            cube_detector.detect_cube_single_thread(images_bgr);
            timer.tac();
        }
    }

    timer.print_statistics();

    return 0;
}
