#!/usr/bin/env python3
"""
Show live stream of the cameras and visualise the specified cube position in
the images.  Press "Q" or "ESC" to quit.
"""
import argparse
import pathlib
import sys

import numpy as np
import cv2
from ament_index_python.packages import get_package_share_directory

import trifinger_object_tracking.py_object_tracker  # noqa
from trifinger_object_tracking.py_tricamera_types import CubeVisualizer
from trifinger_cameras import tricamera
from trifinger_cameras.utils import convert_image


def main():
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        "x",
        type=float,
        help="Target x-position of the cube in world frame.",
    )
    argparser.add_argument(
        "y",
        type=float,
        help="Target y-position of the cube in world frame.",
    )
    argparser.add_argument(
        "z",
        type=float,
        help="Target z-position of the cube in world frame.",
    )
    argparser.add_argument(
        "--multi-process",
        action="store_true",
        help="""If set, use multiprocess camera data to access backend in other
            process.  Otherwise run the backend locally.
        """,
    )
    args = argparser.parse_args()

    camera_names = ["camera60", "camera180", "camera300"]

    cube_position = np.array([args.x, args.y, args.z])
    cube_pose = trifinger_object_tracking.py_object_tracker.ObjectPose()
    cube_pose.position = cube_position
    cube_pose.orientation = np.array([0, 0, 0, 1])

    calib_files = []
    for name in camera_names:
        camera_config_dir = pathlib.Path(
            get_package_share_directory("trifinger_cameras"), "config"
        )

        calib_file = (
            camera_config_dir
            / "camera_calibration_trifingerone"
            / ("%s_cropped.yml" % name)
        )
        if calib_file.exists():
            calib_files.append(str(calib_file))
        else:
            print("{} does not exist.".format(calib_file))
            sys.exit(1)
    cube_visualizer = CubeVisualizer(calib_files)

    if args.multi_process:
        camera_data = tricamera.MultiProcessData("tricamera", False)
    else:
        camera_data = tricamera.SingleProcessData()
        camera_driver = tricamera.TriCameraDriver(
            *camera_names, downsample_images=False
        )
        camera_backend = tricamera.Backend(camera_driver, camera_data)  # noqa

    camera_frontend = tricamera.Frontend(camera_data)

    while True:
        observation = camera_frontend.get_latest_observation()

        images = [
            convert_image(camera.image) for camera in observation.cameras
        ]
        images = cube_visualizer.draw_cube(images, cube_pose, False)

        # show images of all three cameras next to each other
        stacked_image = np.hstack(images)
        cv2.imshow(" | ".join(camera_names), stacked_image)

        # for name, image in zip(camera_names, images):
        #    cv2.imshow(name, image)

        # stop if either "q" or ESC is pressed
        if cv2.waitKey(100) in [ord("q"), 27]:  # 27 = ESC
            break


if __name__ == "__main__":
    main()
