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

    camera_data = tc.tricamera.SingleProcessData()
    tracker_data = ot_tricamera.SingleProcessData()

    logging.debug("create and start loggers")
    camera_logger = tc.tricamera.Logger(camera_data, 1000)
    tracker_logger = ot_tricamera.Logger(tracker_data, 1000)
    camera_logger.start()
    tracker_logger.start()

    logging.debug("create tricamera backend")
    camera_driver = tc.tricamera.TriCameraDriver(*tc.CAMERA_NAMES)
    camera_backend = tc.tricamera.Backend(camera_driver, camera_data)

    logging.debug("create tricameraobjecttracking driver & backend")
    model = object_tracker.get_model_by_name("cube_v2")
    tracker_driver = ot_tricamera.TriCameraObjectTrackerDriver(
        camera_data, cube_model=model
    )
    tracker_backend = ot_tricamera.Backend(tracker_driver, tracker_data)

    # run until logger buffers are full and save to /tmp
    while camera_logger.is_running() or tracker_logger.is_running():
        time.sleep(0.1)

    logging.info("Finished logging.  Write data to /tmp.")
    camera_logger.stop_and_save("/tmp/camera_log.dat")
    tracker_logger.stop_and_save("/tmp/tracker_log.dat")

    logging.info("Shutdown the backends")
    tracker_backend.shutdown()
    camera_backend.shutdown()

    return 0


if __name__ == "__main__":
    with contextlib.suppress(KeyboardInterrupt):
        sys.exit(main())
