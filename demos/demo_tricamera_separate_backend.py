#!/usr/bin/env python3
"""Example on how to connect the driver to an external TriCamera backend."""
import argparse
import contextlib
import logging
import time
import sys

import trifinger_cameras as tc
import trifinger_object_tracking.py_object_tracker as object_tracker
import trifinger_object_tracking.py_tricamera_types as ot_tricamera


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--verbose", "-v", action="store_true", help="Enable verbose output."
    )
    args = parser.parse_args()

    logging.basicConfig(
        level=logging.DEBUG if args.verbose else logging.INFO,
        format="[%(asctime)s] [%(name)s | %(levelname)s] %(message)s",
    )

    # create tricamera backend
    camera_data = tc.tricamera.SingleProcessData()
    camera_driver = tc.tricamera.TriCameraDriver(*tc.CAMERA_NAMES)
    camera_backend = tc.tricamera.Backend(camera_driver, camera_data)

    # create tricameraobjecttracking driver & backend
    model = object_tracker.get_model_by_name("cube_v2")
    tracker_data = ot_tricamera.SingleProcessData()
    tracker_driver = ot_tricamera.TriCameraObjectTrackerDriver(
        camera_data, cube_model=model
    )
    tracker_backend = ot_tricamera.Backend(tracker_driver, tracker_data)

    # create loggers for both
    camera_logger = tc.tricamera.Logger(camera_data, buffer_size=1000)
    tracker_logger = ot_tricamera.Logger(tracker_data, buffer_size=1000)

    # run until logger buffers are full and save to /tmp
    camera_logger.start()
    tracker_logger.start()
    while camera_logger.is_running() or tracker_logger.is_running():
        time.sleep(0.1)

    camera_logger.stop_and_save("/tmp/camera_log.dat")
    tracker_logger.stop_and_save("/tmp/tracker_log.dat")

    camera_backend.shutdown()
    tracker_backend.shutdown()

    return 0


if __name__ == "__main__":
    with contextlib.suppress(KeyboardInterrupt):
        sys.exit(main())
