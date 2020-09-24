#!/usr/bin/env python3
"""Move one finger tip above the object (cube or aruco marker)."""
import argparse
import time
import os

import numpy as np
import rospkg
import cv2
import yaml

import trifinger_simulation
import trifinger_simulation.collision_objects
import trifinger_simulation.finger_types_data
import trifinger_simulation.pinocchio_utils

import robot_interfaces
import robot_fingers

import trifinger_object_tracking.py_object_tracker
import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras.utils import convert_image


def to_matrix(data: dict, key: str) -> np.ndarray:
    return np.array(data[key]["data"]).reshape(data[key]["rows"], data[key]["cols"])


class ArucoDetector:

    def __init__(self, calibration_file):
        # ArUco stuff
        self.marker_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_APRILTAG_16h5)
        self.marker_length = 0.04
        self.marker_id = 0

        with open(calibration_file, "r") as fh:
            camera_calib = yaml.load(fh)

        self.camera_matrix = to_matrix(camera_calib, "camera_matrix")
        self.dist_coeffs = to_matrix(camera_calib, "distortion_coefficients")
        self.trans_world_to_cam = to_matrix(camera_calib, "tf_world_to_camera")
        self.trans_cam_to_world = np.linalg.inv(self.trans_world_to_cam)

    def detect_marker_pose(self, image):
        marker_corners, ids, _ = cv2.aruco.detectMarkers(image, self.marker_dict)
        try:
            i = np.where(ids == self.marker_id)[0][0]
        except Exception:
            return None

        rvecs, tvecs, _ = cv2.aruco.estimatePoseSingleMarkers(
            marker_corners, self.marker_length, self.camera_matrix, self.dist_coeffs
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
    argparser.add_argument("--object-type", choices=["cube", "aruco"],
                           default="cube")
    args = argparser.parse_args()

    robot = robot_fingers.Robot.create_by_name("trifingerpro")

    robot_properties_path = rospkg.RosPack().get_path("robot_properties_fingers")
    urdf_file = trifinger_simulation.finger_types_data.get_finger_urdf("trifingerpro")
    finger_urdf_path = os.path.join(robot_properties_path, "urdf", urdf_file)
    kinematics = trifinger_simulation.pinocchio_utils.Kinematics(
        finger_urdf_path,
        ["finger_tip_link_0", "finger_tip_link_120", "finger_tip_link_240"],
    )

    if args.camera_name:
        camera_index = camera_names.index(args.camera_name)
    else:
        camera_index = None

    camera_data = tricamera.SingleProcessData()
    camera_driver = tricamera.TriCameraObjectTrackerDriver(
        *camera_names,
    )
    camera_backend = tricamera.Backend(  # noqa
        camera_driver, camera_data
    )
    camera_frontend = tricamera.Frontend(camera_data)

    detectors = [
        ArucoDetector("/etc/trifingerpro/camera60_cropped_and_downsampled.yml"),
        ArucoDetector("/etc/trifingerpro/camera180_cropped_and_downsampled.yml"),
        ArucoDetector("/etc/trifingerpro/camera300_cropped_and_downsampled.yml"),
    ]

    robot.initialize()

    init_pos = np.array([0, 1.5, -2.7] * 3)

    for i in range(500):
        finger_action = robot_interfaces.trifinger.Action(position=init_pos)
        robot.frontend.append_desired_action(finger_action)

    joint_pos = init_pos
    i = 0
    while True:
        i += 1

        finger_action = robot_interfaces.trifinger.Action(position=joint_pos)
        finger_action.position_kp = [8] * 9
        finger_action.position_kd = [0.01] * 9
        t = robot.frontend.append_desired_action(finger_action)
        obs = robot.frontend.get_observation(t)

        images = camera_frontend.get_latest_observation()

        if args.object_type == "aruco":
            if camera_index is not None:
                img = convert_image(images.cameras[camera_index].image)
                world_marker_position = detectors[camera_index].detect_marker_pose(img)

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
            0, goal, obs.position, tolerance=0.002, max_iterations=3000)

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


if __name__ == "__main__":
    real()
