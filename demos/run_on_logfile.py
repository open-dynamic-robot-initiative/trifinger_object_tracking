#!/usr/bin/env python3
"""
Run object tracker on a TriCameraObjectObservations log file.
"""
import argparse
import pathlib
import sys

import cv2

import trifinger_object_tracking.py_object_tracker as object_tracker
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras import utils


CAMERA_NAMES = ["camera60", "camera180", "camera300"]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "log_dir",
        type=pathlib.Path,
        help="Path to the log file.",
    )
    args = parser.parse_args()

    camera_log_file = args.log_dir / "camera_data.dat"

    if not args.log_dir.is_dir():
        print("{} is not a directory.".format(args.log_dir))
        sys.exit(1)

    calib_files = []
    for name in CAMERA_NAMES:
        calib_file = args.log_dir / (name + ".yml")
        if calib_file.exists():
            calib_files.append(str(calib_file))
        else:
            print("{} does not exist.".format(calib_file))
            sys.exit(1)
    cube_detector = object_tracker.CubeDetector(calib_files)

    log_reader = tricamera.LogReader(str(camera_log_file))

    for observation in log_reader.data:
        images = [
            utils.convert_image(camera.image) for camera in observation.cameras
        ]

        pose = cube_detector.detect_cube_single_thread(images)

        print(pose.position)

        debug_img = cube_detector.create_debug_image()
        cv2.imshow("Debug", debug_img)
        # stop if either "q" or ESC is pressed
        if cv2.waitKey(3) in [ord("q"), 27]:  # 27 = ESC
            break


if __name__ == "__main__":
    main()
