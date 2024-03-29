#!/usr/bin/env python3
"""Move one finger tip above the object (cube or aruco marker)."""
import argparse
import time
import os

import numpy as np
import cv2
import yaml

from ament_index_python.packages import get_package_share_directory

import trifinger_simulation
import trifinger_simulation.collision_objects
import trifinger_simulation.finger_types_data
import robot_interfaces
import robot_fingers
from robot_properties_fingers import pinocchio_utils

import trifinger_object_tracking.py_object_tracker
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras.utils import convert_image


def to_matrix(data: dict, key: str) -> np.ndarray:
    return np.array(data[key]["data"]).reshape(
        data[key]["rows"], data[key]["cols"]
    )


class ArucoDetector:
    def __init__(self, calibration_file):
        # ArUco stuff
        self.marker_dict = cv2.aruco.getPredefinedDictionary(
            cv2.aruco.DICT_APRILTAG_16h5
        )
        self.marker_length = 0.04
        self.marker_id = 0

        with open(calibration_file, "r") as fh:
            camera_calib = yaml.load(fh)

        self.camera_matrix = to_matrix(camera_calib, "camera_matrix")
        self.dist_coeffs = to_matrix(camera_calib, "distortion_coefficients")
        self.trans_world_to_cam = to_matrix(camera_calib, "tf_world_to_camera")
        self.trans_cam_to_world = np.linalg.inv(self.trans_world_to_cam)

    def detect_marker_pose(self, image):
        marker_corners, ids, _ = cv2.aruco.detectMarkers(
            image, self.marker_dict
        )
        try:
            i = np.where(ids == self.marker_id)[0][0]
        except Exception:
            return None

        rvecs, tvecs, _ = cv2.aruco.estimatePoseSingleMarkers(
            marker_corners,
            self.marker_length,
            self.camera_matrix,
            self.dist_coeffs,
        )

        # transform marker position from camera to world frame
        cam_marker_position = np.ones(4)
        cam_marker_position[:3] = tvecs[i]
        world_marker_position = self.trans_cam_to_world @ cam_marker_position

        return world_marker_position


def real():
    camera_names = ["camera60", "camera180", "camera300"]

    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument("--camera-name", "-c", choices=camera_names)
    argparser.add_argument(
        "--object-type", choices=["cube", "aruco"], default="cube"
    )
    argparser.add_argument(
        "--cube-model",
        type=str,
        default="cube_v2",
        help="""Name of the model used for cube detection (only used if
            --object-type=cube.  Default: %(default)s.
        """,
    )
    argparser.add_argument("--no-downsample", action="store_true")
    argparser.add_argument("--multi-process", action="store_true")
    args = argparser.parse_args()

    robot_properties_path = get_package_share_directory(
        "robot_properties_fingers"
    )
    urdf_file = trifinger_simulation.finger_types_data.get_finger_urdf(
        "trifingerpro"
    )
    finger_urdf_path = os.path.join(robot_properties_path, "urdf", urdf_file)
    kinematics = pinocchio_utils.Kinematics(
        finger_urdf_path,
        ["finger_tip_link_0", "finger_tip_link_120", "finger_tip_link_240"],
    )

    if args.camera_name:
        camera_index = camera_names.index(args.camera_name)
    else:
        camera_index = None

    if args.multi_process:
        camera_data = tricamera.MultiProcessData("tricamera", False)
    else:
        camera_data = tricamera.SingleProcessData()

        model = trifinger_object_tracking.py_object_tracker.get_model_by_name(
            args.cube_model
        )
        camera_driver = tricamera.TriCameraObjectTrackerDriver(
            *camera_names,
            cube_model=model,
            downsample_images=(not args.no_downsample)
        )
        camera_backend = tricamera.Backend(camera_driver, camera_data)
    camera_frontend = tricamera.Frontend(camera_data)

    if args.object_type == "aruco":
        if args.no_downsample:
            calib_file_fmt = "/etc/trifingerpro/camera{}_cropped.yml"
        else:
            calib_file_fmt = (
                "/etc/trifingerpro/camera{}_cropped_and_downsampled.yml"
            )

        detectors = [
            ArucoDetector(calib_file_fmt.format(60)),
            ArucoDetector(calib_file_fmt.format(180)),
            ArucoDetector(calib_file_fmt.format(300)),
        ]

    if args.multi_process:
        robot_data = robot_interfaces.trifinger.MultiProcessData(
            "trifinger", False
        )
        frontend = robot_interfaces.trifinger.Frontend(robot_data)
    else:
        robot = robot_fingers.Robot.create_by_name("trifingerpro")
        robot.initialize()
        frontend = robot.frontend

    init_pos = np.array([0, 1.5, -2.7] * 3)

    for i in range(500):
        finger_action = robot_interfaces.trifinger.Action(position=init_pos)
        frontend.append_desired_action(finger_action)

    joint_pos = init_pos
    i = 0
    while True:
        i += 1

        finger_action = robot_interfaces.trifinger.Action(position=joint_pos)
        finger_action.position_kp = [8] * 9
        finger_action.position_kd = [0.01] * 9
        t = frontend.append_desired_action(finger_action)
        obs = frontend.get_observation(t)

        images = camera_frontend.get_latest_observation()

        if args.object_type == "aruco":
            if camera_index is not None:
                img = convert_image(images.cameras[camera_index].image)
                world_marker_position = detectors[
                    camera_index
                ].detect_marker_pose(img)

                if world_marker_position is None:
                    time.sleep(0.1)
                    continue
            else:
                positions = [
                    detector.detect_marker_pose(convert_image(camera.image))
                    for detector, camera in zip(detectors, images.cameras)
                ]
                # filter out Nones
                positions = [p for p in positions if p is not None]
                if not positions:
                    time.sleep(0.1)
                    continue

                world_marker_position = np.mean(positions, axis=0)
        else:
            world_marker_position = images.object_pose.position

        # set goal a bit above the marker
        goal = np.array(world_marker_position[:3], copy=True)
        goal[2] += 0.08

        new_joint_pos, err = kinematics.inverse_kinematics_one_finger(
            0, goal, obs.position, tolerance=0.002, max_iterations=3000
        )

        # keep the other two fingers up
        alpha = 0.1
        joint_pos[:3] = alpha * new_joint_pos[:3] + (1 - alpha) * joint_pos[:3]

        if i % 500 == 0:
            tip_pos = kinematics.forward_kinematics(obs.position)

            print("-----------------------------------------------------")
            print("Object position:", np.round(world_marker_position[:3], 3))
            print("Tip goal:", np.round(goal, 3))
            print("Tip position:", np.round(tip_pos[0], 3))
            print("IK error:", np.round(err, 3))

    if not args.multi_process:
        camera_backend.shutdown()


if __name__ == "__main__":
    real()
