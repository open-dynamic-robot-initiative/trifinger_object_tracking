#!/usr/bin/env python3
"""Record a number of debug images and save them to files.

Maintenance script to record a number of debug images (visualising results of the object
detection) and save them to a specified directory.
"""
import argparse
import logging
import pathlib
import sys
import typing

import tomli
import cv2
import numpy as np

import trifinger_object_tracking.py_tricamera_types as tricamera
import trifinger_object_tracking.py_object_tracker as object_tracker


def load_object_type() -> typing.Optional[str]:
    """Load object type from submission system config file."""
    _submission_system_config_file = "/etc/trifingerpro/submission_system.toml"
    with open(_submission_system_config_file, "rb") as f:
        config = tomli.load(f)

    try:
        return config["post_submission"]["object_type"]
    except KeyError:
        print("ERROR: failed to load object from config file.")
        return None


def record_debug_images(
    object_type: str, num_images: int
) -> typing.List[np.ndarray]:
    """Record a number of debug images.

    Args:
        object_type: Which object to look for ("cube" or "cuboid").
        num_images: How many images to record.

    Returns:
        List of debug images.
    """
    logger = logging.getLogger("record_debug_images")

    object_models = {
        "cube": "cube_v2",
        "cuboid": "cuboid_2x2x8_v2",
    }

    logger.debug(
        "Create camera driver for object model %s", object_models[object_type]
    )

    model = object_tracker.get_model_by_name(object_models[object_type])
    camera_driver = tricamera.TriCameraObjectTrackerDriver(
        "camera60", "camera180", "camera300", model
    )

    # collect observations
    observation_buffer: typing.List[np.ndarray] = []
    while len(observation_buffer) < num_images:
        logger.debug("Fetch debug image")
        obs = camera_driver.get_debug_image()
        observation_buffer.append(obs)

    return observation_buffer


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "output_dir",
        type=pathlib.Path,
        help="Directory in which output files are saved.",
    )
    parser.add_argument(
        "--num-images",
        "-n",
        type=int,
        default=1,
        help="Number of images that are recorded.",
    )
    parser.add_argument(
        "--object",
        "-o",
        type=str,
        choices=["cube", "cuboid", "auto"],
        default="auto",
        help="""Specify with which object the robot is equipped (if any).  If
            set to "auto", the object type is read from the submission system
            configuration.
        """,
    )
    parser.add_argument(
        "--file-pattern",
        "-p",
        type=str,
        default="image%02d.png",
        help="Pattern used for saving the images.  Default: %(default)s",
    )
    args = parser.parse_args()

    logging.basicConfig(
        level=logging.DEBUG,
        format="[%(asctime)s] [%(name)s] [%(levelname)s] %(message)s",
    )
    logger = logging.getLogger("record_debug_images")

    if not args.output_dir.is_dir():
        print(f"ERROR: {args.output_dir} does not exist or is not a directory")
        return 1

    if args.object == "auto":
        args.object = load_object_type()
        logger.info("Auto-detected object type %s", args.object)

    images = record_debug_images(args.object, args.num_images)

    for i, image in enumerate(images):
        filename = str(args.output_dir / (args.file_pattern % i))
        logger.info("Save image %s", filename)
        cv2.imwrite(filename, image)

    return 0


if __name__ == "__main__":
    sys.exit(main())
