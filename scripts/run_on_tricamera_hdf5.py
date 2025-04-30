#!/usr/bin/env python3
"""Run object tracker on a TriCamera HDF5 log file."""

import argparse
import pathlib
import sys
import json
import time
import statistics

import cv2
import h5py

import trifinger_object_tracking.py_object_tracker as object_tracker
from trifinger_cameras import hdf5, CAMERA_NAMES
from trifinger_cameras.py_camera_types import CameraParameters


def construct_camera_parameters(
    h5file: h5py.File, camera_name: str
) -> CameraParameters:
    """Construct CameraParameters from data in HDF5 file.

    Args:
        h5file: Opened HDF5 file.
        camera_name: Name of the camera.

    Returns:
        Camera parameters of the specified camera.
    """
    h5_camera_info = h5file["camera_info"][camera_name]

    params = CameraParameters()
    params.image_width = h5file.attrs["image_width"]
    params.image_height = h5file.attrs["image_height"]
    params.camera_matrix = h5_camera_info["camera_matrix"][:]
    params.distortion_coefficients = h5_camera_info["distortion_coefficients"][
        :
    ]
    params.tf_world_to_camera = h5_camera_info["tf_world_to_camera"][:]
    return params


def main() -> None:
    """Entry point."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "logfile",
        type=pathlib.Path,
        help="Path to the HDF5 log file.",
    )
    parser.add_argument(
        "--demosaicing",
        type=str,
        choices=("bilinear", "vng", "ea"),
        default="bilinear",
        help="Algorithm used for demosaicing.",
    )
    parser.add_argument(
        "--no-visualization",
        action="store_true",
        help="Disable displaying visualization images.",
    )
    parser.add_argument(
        "-q",
        "--quiet",
        action="store_true",
        help="Disable printing pose information.",
    )
    parser.add_argument(
        "--summary",
        action="store_true",
        help="Show timing statistics at the end.",
    )
    args = parser.parse_args()

    if not args.logfile.is_file():
        print(f"{args.logfile} does not exist.")
        sys.exit(1)

    # default is bilinear
    demosaicing_flag = cv2.COLOR_BAYER_BG2BGR
    match args.demosaicing:
        case "vng":
            demosaicing_flag = cv2.COLOR_BAYER_BG2BGR_VNG
        case "ea":
            demosaicing_flag = cv2.COLOR_BAYER_BG2BGR_EA

    # Open HDF5 file
    with h5py.File(args.logfile, "r") as h5file:
        try:
            hdf5.verify_tricamera_hdf5(h5file, supported_formats=(2,))
        except ValueError as e:
            print(e, file=sys.stderr)
            sys.exit(1)

        # Create cube detector with calibration parameters
        camera_params = [
            construct_camera_parameters(h5file, camera_name)
            for camera_name in CAMERA_NAMES
        ]
        cube_model = object_tracker.CubeV2Model()
        cube_detector = object_tracker.CubeDetector(cube_model, camera_params)

        # Get images from HDF5
        n_frames = h5file["images"].shape[0]
        detection_times_ms = []
        for frame_idx in range(n_frames):
            # Get images for all three cameras
            images = [
                cv2.demosaicing(
                    h5file["images"][frame_idx, i], demosaicing_flag
                )
                for i in range(len(CAMERA_NAMES))
            ]

            start_time = time.monotonic()
            pose = cube_detector.detect_cube_single_thread(images)
            detection_time_ms = (time.monotonic() - start_time) * 1000
            detection_times_ms.append(detection_time_ms)

            if not args.quiet:
                pose_data = {
                    "position": pose.position.tolist(),
                    "orientation": pose.orientation.tolist(),
                    "confidence": pose.confidence,
                    "duration_ms": detection_time_ms,
                }
                print(json.dumps(pose_data))

            if not args.no_visualization:
                debug_img = cube_detector.create_debug_image()
                cv2.imshow("Debug", debug_img)
                # stop if either "q" or ESC is pressed
                if cv2.waitKey(3) in [ord("q"), 27]:  # 27 = ESC
                    break

        if args.summary:
            mean_time = statistics.mean(detection_times_ms)
            std_time = statistics.stdev(detection_times_ms)
            print(
                "\nTiming Summary:",
                f"\n  Mean detection time: {mean_time:.0f} ms",
                f"\n  Std dev: {std_time:.0f} ms",
                f"\n  Total frames: {len(detection_times_ms)}",
                file=sys.stderr,
            )


if __name__ == "__main__":
    main()
