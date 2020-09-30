#!/usr/bin/env python3
"""
Play back TriCameraObjectObservations from a log file.
"""
import argparse
import cv2

import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras import utils


def main():
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        "filename",
        type=str,
        help="""Path to the log file.""",
    )
    args = argparser.parse_args()

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
        window_60 = "Image Stream camera60"
        window_180 = "Image Stream camera180"
        window_300 = "Image Stream camera300"
        cv2.imshow(
            window_60, utils.convert_image(observation.cameras[0].image)
        )
        cv2.imshow(
            window_180, utils.convert_image(observation.cameras[1].image)
        )
        cv2.imshow(
            window_300, utils.convert_image(observation.cameras[2].image)
        )

        # stop if either "q" or ESC is pressed
        if cv2.waitKey(interval) in [ord("q"), 27]:  # 27 = ESC
            break


if __name__ == "__main__":
    main()
