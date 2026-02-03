#!/usr/bin/env python3
"""Demo on how to access observations from the TriCameraObjectTrackerDriver."""

import argparse
import pathlib
import sys
import time

import cv2
import numpy as np

import signal_handler
import trifinger_object_tracking.py_object_tracker as object_tracker
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras import CAMERA_NAMES, utils


calib_files = [
    pathlib.Path(f"/etc/trifingerpro/{camera}_cropped_and_downsampled.yml")
    for camera in CAMERA_NAMES
]


def back_end_only_loop() -> None:
    """Idle loop for the back-end-only mode."""
    print("Backend is running.  Press Ctrl-C to stop.")
    signal_handler.init()
    while not signal_handler.has_received_sigint():
        time.sleep(1)


def front_end_loop(
    camera_data: tricamera.BaseData, model: object_tracker.BaseCuboidModel
) -> None:
    """Run the front end to display camera images with visualized object pose."""
    camera_frontend = tricamera.Frontend(camera_data)

    cube_visualizer = tricamera.CubeVisualizer(
        model, [str(f) for f in calib_files]
    )

    print("=== Camera Info: ===")
    utils.print_tricamera_sensor_info(camera_frontend.get_sensor_info())

    last_print = time.time()
    while True:
        observation = camera_frontend.get_latest_observation()
        images = [
            utils.convert_image(camera.image) for camera in observation.cameras
        ]
        images = cube_visualizer.draw_cube(
            images, observation.object_pose, False
        )

        stacked_image = np.hstack(images)
        cv2.imshow(" | ".join(CAMERA_NAMES), stacked_image)

        now = time.time()
        if (now - last_print) >= 1.0:
            print("-----")
            print(
                "Object Pose Confidence:", observation.object_pose.confidence
            )
            print("Object Position:", observation.object_pose.position)
            print("Object Orientation:", observation.object_pose.orientation)
            last_print = now

        # stop if either "q" or ESC is pressed
        if cv2.waitKey(3) in [ord("q"), 27]:  # 27 = ESC
            break


def main() -> int:  # noqa: D103
    parser = argparse.ArgumentParser(description=__doc__)

    mode_group = parser.add_mutually_exclusive_group()
    mode_group.add_argument(
        "--frontend-only",
        action="store_const",
        dest="mode",
        const="frontend-only",
        help="""Run only the front end, using multi-process camera data.  In this case,
            the back end has to be run in a separate process.
        """,
    )
    mode_group.add_argument(
        "--backend-only",
        action="store_const",
        dest="mode",
        const="backend-only",
        help="""Run only the back end, using multi-process camera data.  In this case,
            the front end has to be run in a separate process.
        """,
    )
    mode_group.add_argument(
        "--single-process",
        action="store_const",
        dest="mode",
        const="single-process",
        help="Run both front and back end, using single-process camera data.",
    )
    mode_group.set_defaults(mode="single-process")

    parser.add_argument(
        "--object",
        type=str,
        default="cube_v2",
        help="Name of the model used for object detection.  Default: %(default)s.",
    )
    args = parser.parse_args()

    model = object_tracker.get_model_by_name(args.object)

    if args.mode == "single-process":
        camera_data = tricamera.SingleProcessData()
    else:
        camera_data = tricamera.MultiProcessData("tricamera", False)

    if args.mode != "frontend-only":
        camera_driver = tricamera.TriCameraObjectTrackerDriver(
            *calib_files, cube_model=model
        )
        camera_backend = tricamera.Backend(camera_driver, camera_data)
    else:
        camera_backend = None

    if args.mode == "backend-only":
        back_end_only_loop()
    else:
        front_end_loop(camera_data, model)

    if camera_backend is not None:
        camera_backend.shutdown()

    return 0


if __name__ == "__main__":
    sys.exit(main())
