#!/usr/bin/env python3
"""Convert TriCameraWithObjectObservation log file to hdf5."""
# This is based on the equivalent script in trifinger_cameras.

import argparse
import pathlib
import sys

import h5py
import numpy as np

from trifinger_cameras import CAMERA_NAMES
from trifinger_cameras.camera_calibration_file import CameraCalibrationFile
from trifinger_cameras.hdf5 import write_tricamera_hdf5

import trifinger_object_tracking.py_tricamera_types as tricamera


def main() -> int:
    """Main entry point of the script."""
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        "--logfile",
        "-l",
        type=pathlib.Path,
        required=True,
        help="Path to the log file.",
    )
    argparser.add_argument(
        "--outfile",
        "-o",
        type=pathlib.Path,
        required=True,
        help="Path to the output hdf5 file.",
    )
    # Add arguments for calibration files
    argparser.add_argument(
        "--camera-info",
        "-c",
        type=pathlib.Path,
        nargs=3,
        required=True,
        help="Paths to the three camera calibration YAML files.",
    )
    args = argparser.parse_args()

    if not args.logfile.is_file():
        print("Log file does not exist.", file=sys.stderr)
        return 1

    if args.outfile.exists():
        print("Output file already exists.  Exiting.", file=sys.stderr)
        return 1

    # Load calibration files
    camera_params = []
    for calib_file in args.camera_info:
        if not calib_file.is_file():
            print(
                f"Calibration file {calib_file} does not exist.",
                file=sys.stderr,
            )
            return 1
        camera_params.append(CameraCalibrationFile(calib_file))

    # Verify camera names match expected order
    for param, expected_name in zip(camera_params, CAMERA_NAMES, strict=True):
        if param["camera_name"] != expected_name:
            print(
                f"Expected camera {expected_name} but got {param['camera_name']}",
                file=sys.stderr,
            )
            return 1

    log_reader = tricamera.LogReader(str(args.logfile))

    n_frames = len(log_reader.data)
    assert n_frames > 0, "No frames found in log file."

    img_shape = log_reader.data[0].cameras[0].image.shape

    # sanity check that the image size matches with camera info files
    for i, params in enumerate(camera_params):
        if (
            params["image_width"] != img_shape[1]
            or params["image_height"] != img_shape[0]
        ):
            print(
                f"Image size mismatch for camera {CAMERA_NAMES[i]}:"
                f" expected {img_shape[1]}x{img_shape[0]} (based on camera info) but"
                f" got {params['image_width']}x{params['image_height']}",
                file=sys.stderr,
            )
            return 1

    with h5py.File(args.outfile, "w") as h5:
        write_tricamera_hdf5(
            h5,
            camera_params,
            n_frames,
            zip(log_reader.data, log_reader.timestamps, strict=True),
        )

        # Add the object information
        object_group = h5.create_group("object_pose")
        object_group.create_dataset(
            "position",
            data=[obs.object_pose.position for obs in log_reader.data],
            dtype=np.double,
        )
        object_group.create_dataset(
            "orientation",
            data=[obs.object_pose.orientation for obs in log_reader.data],
            dtype=np.double,
        )
        object_group.create_dataset(
            "confidence",
            data=[obs.object_pose.confidence for obs in log_reader.data],
            dtype=np.double,
        )

    return 0


if __name__ == "__main__":
    sys.exit(main())
