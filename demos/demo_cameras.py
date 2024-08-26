#!/usr/bin/env python3
"""Demo on how to access observations from the TriCameraObjectTrackerDriver."""

import argparse
import time

import cv2
import numpy as np

import trifinger_object_tracking.py_object_tracker
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras import utils


def main() -> None:  # noqa: D103
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        "--multi-process",
        action="store_true",
        help="""If set, use multiprocess camera data to access backend in other
            process.  Otherwise run the backend locally.
        """,
    )
    argparser.add_argument(
        "--object",
        type=str,
        default="cube_v2",
        help="Name of the model used for object detection.  Default: %(default)s.",
    )
    args = argparser.parse_args()

    camera_names = ["camera60", "camera180", "camera300"]

    model = trifinger_object_tracking.py_object_tracker.get_model_by_name(args.object)

    if args.multi_process:
        camera_data = tricamera.MultiProcessData("tricamera", False)
    else:
        camera_data = tricamera.SingleProcessData()
        camera_driver = tricamera.TriCameraObjectTrackerDriver(
            *camera_names, cube_model=model
        )
        camera_backend = tricamera.Backend(camera_driver, camera_data)

    camera_frontend = tricamera.Frontend(camera_data)

    calib_files = [
        "/etc/trifingerpro/camera60_cropped_and_downsampled.yml",
        "/etc/trifingerpro/camera180_cropped_and_downsampled.yml",
        "/etc/trifingerpro/camera300_cropped_and_downsampled.yml",
    ]
    cube_visualizer = tricamera.CubeVisualizer(model, calib_files)

    np.set_printoptions(precision=3, suppress=True)
    print("=== Camera Info: ======================")
    for i, info in enumerate(camera_frontend.get_sensor_info().camera):
        print(f"--- Camera {i}: ----------------------")
        print(f"fps: {info.frame_rate_fps}")
        print(f"width x height: {info.image_width}x{info.image_height}")
        print(info.camera_matrix)
        print(info.distortion_coefficients)
        print(info.tf_world_to_camera)
        print("---------------------------------------")
    print("=======================================")

    last_print = time.time()
    while True:
        observation = camera_frontend.get_latest_observation()
        images = [utils.convert_image(camera.image) for camera in observation.cameras]
        images = cube_visualizer.draw_cube(images, observation.object_pose, False)

        stacked_image = np.hstack(images)
        cv2.imshow(" | ".join(camera_names), stacked_image)

        now = time.time()
        if (now - last_print) >= 1.0:
            print("-----")
            print("Object Pose Confidence:", observation.object_pose.confidence)
            print("Object Position:", observation.object_pose.position)
            print("Object Orientation:", observation.object_pose.orientation)
            last_print = now

        # stop if either "q" or ESC is pressed
        if cv2.waitKey(3) in [ord("q"), 27]:  # 27 = ESC
            break

    if not args.multi_process:
        camera_backend.shutdown()


if __name__ == "__main__":
    main()
