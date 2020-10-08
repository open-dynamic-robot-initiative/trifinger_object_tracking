/**
 * @file
 * @brief Wrapper on the Pylon Driver to synchronise three pylon dependent
 * cameras.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 */
#include <trifinger_object_tracking/pybullet_tricamera_object_tracker_driver.hpp>

#include <chrono>
#include <thread>

#include <pybind11/eigen.h>
#include <pybind11/embed.h>
#include <serialization_utils/cereal_cvmat.hpp>

namespace py = pybind11;

namespace trifinger_object_tracking
{
trifinger_object_tracking::TriCameraObjectObservation
PyBulletTriCameraObjectTrackerDriver::get_observation()
{
    time_series::Index robot_t =
        robot_data_->observation->newest_timeindex(false);
    if (robot_t == time_series::EMPTY)
    {
        // As long as robot backend did not start yet, run at around 10 Hz
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
    }
    else
    {
        // Synchronize with robot backend:  To achieve 10 Hz, there should be
        // one camera observation every 100 robot steps.
        while (robot_t < last_update_robot_time_index_ + 100)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
            auto new_robot_t =
                robot_data_->observation->newest_timeindex(false);

            // If robot t did not increase, assume the robot has stopped.
            // Break this look to avoid a dead-lock.
            if (new_robot_t == robot_t)
            {
                break;
            }
            robot_t = new_robot_t;
        }
        last_update_robot_time_index_ = robot_t;
    }

    trifinger_object_tracking::TriCameraObjectObservation observation;

    auto current_time = std::chrono::system_clock::now();
    double timestamp =
        std::chrono::duration<double>(current_time.time_since_epoch()).count();
    for (int i = 0; i < 3; i++)
    {
        observation.cameras[i].timestamp = timestamp;
    }

    {
        py::gil_scoped_acquire acquire;

        if (render_images_)
        {
            py::list images = cameras_.attr("get_bayer_images")();
            for (int i = 0; i < 3; i++)
            {
                // ensure that the image array is contiguous in memory,
                // otherwise conversion to cv::Mat would fail
                auto image = numpy_.attr("ascontiguousarray")(images[i]);
                // convert to cv::Mat
                image = cvMat_(image);
                observation.cameras[i].image = image.cast<cv::Mat>();
            }
        }

        pybind11::tuple state = tracking_object_.attr("get_state")();
        observation.object_pose.position = state[0].cast<Eigen::Vector3d>();
        observation.object_pose.orientation = state[1].cast<Eigen::Vector4d>();
    }

    // there is no noise in the simulation
    observation.object_pose.confidence = 1.0;

    return observation;
}

}  // namespace trifinger_object_tracking
