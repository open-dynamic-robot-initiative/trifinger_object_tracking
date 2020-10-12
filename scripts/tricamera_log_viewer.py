#!/usr/bin/env python3
"""
Play back TriCameraObjectObservations from a log file.
"""
import argparse
import pathlib
import sys

import numpy as np
import cv2

import trifinger_cameras
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras import utils


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "filename",
        type=str,
        help="Path to the log file.",
    )
    parser.add_argument(
        "--visualize-object-pose",
        "-v",
        action="store_true",
        help="""Visualize detected object pose.  This expects files
        camera{60,180,300}.yml with calibration parameters to exist in the same
        directory as the given camera log file.
        """,
    )
    parser.add_argument(
        "--show-confidence",
        action="store_true",
        help="Print the object pose confidence in the images.",
    )
    parser.add_argument(
        "--unfiltered",
        action="store_true",
        help="Use the unfiltered object pose.",
    )
    args = parser.parse_args()

    log_file_path = pathlib.Path(args.filename)

    if not log_file_path.exists():
        print("{} does not exist.".format(log_file_path))
        sys.exit(1)

    calib_files = []
    if args.visualize_object_pose:
        for name in ("camera60", "camera180", "camera300"):
            calib_file = log_file_path.parent / (name + ".yml")
            if calib_file.exists():
                calib_files.append(str(calib_file))
            else:
                print("{} does not exist.".format(calib_file))
                sys.exit(1)
        cube_visualizer = tricamera.CubeVisualizer(calib_files)

    log_reader = tricamera.LogReader(args.filename)

    # determine rate based on time stamps
    start_time = log_reader.data[0].cameras[0].timestamp
    end_time = log_reader.data[-1].cameras[0].timestamp
    interval = (end_time - start_time) / len(log_reader.data)
    # convert to ms
    interval = int(interval * 1000)

    print(
        "Loaded {} frames at an average interval of {} ms ({:.1f} fps)".format(
            len(log_reader.data), interval, 1000 / interval
        )
    )

    for observation in log_reader.data:
        images = [utils.convert_image(camera.image) for camera in observation.cameras]

        if args.unfiltered:
            object_pose = observation.object_pose
        else:
            object_pose = observation.filtered_object_pose

        if args.visualize_object_pose:
            cvmats = [trifinger_cameras.camera.cvMat(img) for img in images]
            images = cube_visualizer.draw_cube(cvmats, object_pose, False)
            images = [np.array(img) for img in images]

        if args.show_confidence:
            images = [cv2.putText(
                image,
                "confidence: %.2f" % object_pose.confidence,
                (0, image.shape[0] - 10),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                (255, 255, 0)
            ) for image in images]

        for i, name in enumerate(["camera60", "camera180", "camera300"]):
            cv2.imshow(name, images[i])

        # stop if either "q" or ESC is pressed
        if cv2.waitKey(interval) in [ord("q"), 27]:  # 27 = ESC
            break

        # for testing
        # if object_pose.confidence < 0.9:
        #     cv2.waitKey(0)


if __name__ == "__main__":
    main()
