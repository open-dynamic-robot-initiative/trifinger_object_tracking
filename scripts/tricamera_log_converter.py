#!/usr/bin/env python3
"""
Convert images from a TriCameraObjectObservation log file to a video file.
"""
import argparse
import cv2

import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras import utils


def main():
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        "logfile", type=str, help="Path to the log file.",
    )
    argparser.add_argument(
        "outfile", type=str, help="Path to the output file.",
    )
    argparser.add_argument(
        "--camera",
        "-c",
        choices=["camera60", "camera180", "camera300"],
        required=True,
        help="Name of the camera",
    )
    args = argparser.parse_args()

    log_reader = tricamera.LogReader(args.logfile)

    if args.camera == "camera60":
        camera_idx = 0
    elif args.camera == "camera180":
        camera_idx = 1
    else:
        camera_idx = 2

    first_obs = log_reader.data[0].cameras[camera_idx]
    last_obs = log_reader.data[-1].cameras[camera_idx]

    # determine rate based on time stamps
    start_time = first_obs.timestamp
    end_time = last_obs.timestamp
    interval = (end_time - start_time) / len(log_reader.data)
    fps = 1 / interval
    # convert to ms
    interval = int(interval * 1000)

    # Define the codec and create VideoWriter object
    first_img = utils.convert_image(first_obs.image)
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    writer = cv2.VideoWriter(args.outfile, fourcc, fps, first_img.shape[:2])

    print(
        "Loaded {} frames at an average interval of {} ms ({:.1f} fps)".format(
            len(log_reader.data), interval, 1000 / interval
        )
    )

    for observation in log_reader.data:
        image = utils.convert_image(observation.cameras[camera_idx].image)
        writer.write(image)


if __name__ == "__main__":
    main()
