#!/usr/bin/env python3
"""
Demo on how to access observations from the TriCameraObjectTrackerDriver.
"""
import argparse
import cv2

import trifinger_object_tracking.py_object_tracker
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras.utils import convert_image


def main():
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

    if args.multi_process:
        camera_data = tricamera.MultiProcessData("tricamera", False)
    else:
        camera_data = tricamera.SingleProcessData()

        model = trifinger_object_tracking.py_object_tracker.get_model_by_name(
            args.object
        )
        camera_driver = tricamera.TriCameraDriver(
            *camera_names, cube_model=model
        )
        camera_backend = tricamera.Backend(camera_driver, camera_data)

    camera_frontend = tricamera.Frontend(camera_data)

    while True:
        observation = camera_frontend.get_latest_observation()
        for i, name in enumerate(camera_names):
            cv2.imshow(name, convert_image(observation.cameras[i].image))

        print("-----")
        print("Object Pose Confidence:", observation.object_pose.confidence)
        print("Object Position:", observation.object_pose.position)
        print("Object Orientation:", observation.object_pose.orientation)

        # stop if either "q" or ESC is pressed
        if cv2.waitKey(3) in [ord("q"), 27]:  # 27 = ESC
            break

    if not args.multi_process:
        camera_backend.shutdown()


if __name__ == "__main__":
    main()
