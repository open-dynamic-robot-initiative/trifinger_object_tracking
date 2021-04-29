#!/usr/bin/env python3
"""
Extract object poses from a TriCameraObjectObservations log file.

The extracted poses are written to a simple CSV file.
"""
import argparse
import pathlib

import pandas
import progressbar

import trifinger_object_tracking.py_tricamera_types as tricamera


def main():
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        "input",
        type=str,
        help="""Path to the log file.""",
    )
    argparser.add_argument(
        "output",
        type=pathlib.Path,
        help="Directory to which images are stored.",
    )
    args = argparser.parse_args()

    log_reader = tricamera.LogReader(args.input)

    object_log = pandas.DataFrame(
        columns=(
            "timestamp",
            "pos_x",
            "pos_y",
            "pos_z",
            "rot_x",
            "rot_y",
            "rot_z",
            "rot_q",
            "confidence",
            "filtered_pos_x",
            "filtered_pos_y",
            "filtered_pos_z",
            "filtered_rot_x",
            "filtered_rot_y",
            "filtered_rot_z",
            "filtered_rot_q",
            "filtered_confidence",
        )
    )

    pbar = progressbar.ProgressBar()
    for i, observation in enumerate(pbar(log_reader.data)):
        # use timestamp of camera 0
        timestamp = observation.cameras[0].timestamp

        object_pose = observation.object_pose
        filtered_object_pose = observation.filtered_object_pose

        row = {
            "timestamp": timestamp,
            "pos_x": object_pose.position[0],
            "pos_y": object_pose.position[1],
            "pos_z": object_pose.position[2],
            "rot_x": object_pose.orientation[0],
            "rot_y": object_pose.orientation[1],
            "rot_z": object_pose.orientation[2],
            "rot_q": object_pose.orientation[3],
            "confidence": object_pose.confidence,
            "filtered_pos_x": filtered_object_pose.position[0],
            "filtered_pos_y": filtered_object_pose.position[1],
            "filtered_pos_z": filtered_object_pose.position[2],
            "filtered_rot_x": filtered_object_pose.orientation[0],
            "filtered_rot_y": filtered_object_pose.orientation[1],
            "filtered_rot_z": filtered_object_pose.orientation[2],
            "filtered_rot_q": filtered_object_pose.orientation[3],
            "filtered_confidence": filtered_object_pose.confidence,
        }
        object_log = object_log.append(row, ignore_index=True)

    object_log.to_csv(args.output)


if __name__ == "__main__":
    main()
