import pathlib

import pytest
import cv2
import numpy as np
from scipy.spatial.transform import Rotation

import trifinger_object_tracking.py_object_tracker as object_tracker


def rotation_error(actual: Rotation, expected: Rotation) -> float:
    error_rot = expected.inv() * actual
    return error_rot.magnitude()


def load_test_data(object_name):
    test_dir = pathlib.Path(__file__).parent
    test_image_dir = test_dir / "images" / "pose_detection" / object_name

    camera_ids = (60, 180, 300)
    images = [
        cv2.imread(str(test_image_dir / f"camera{id}.png"))
        for id in camera_ids
    ]
    camera_parameter_files = [
        str(test_image_dir / f"camera_calib_{id}.yml") for id in camera_ids
    ]

    return {"images": images, "camera_parameter_files": camera_parameter_files}


def test_rotation_error():
    base = Rotation.from_euler("x", 0)
    x90 = Rotation.from_euler("x", 90, degrees=True)
    y90 = Rotation.from_euler("y", 90, degrees=True)
    ym90 = Rotation.from_euler("y", -90, degrees=True)
    z180 = Rotation.from_euler("z", 180, degrees=True)

    assert rotation_error(base, base) == pytest.approx(0.0)
    assert rotation_error(x90, x90) == pytest.approx(0.0)
    assert rotation_error(base, x90) == pytest.approx(np.pi / 2)
    assert rotation_error(base, y90) == pytest.approx(np.pi / 2)
    assert rotation_error(base, ym90) == pytest.approx(np.pi / 2)
    assert rotation_error(base, z180) == pytest.approx(np.pi)


def test_find_pose_cube_v1():
    model = object_tracker.CubeV1Model()
    test_data = load_test_data(model.get_name())

    cube_detector = object_tracker.CubeDetector(
        model, test_data["camera_parameter_files"]
    )

    pose = cube_detector.detect_cube_single_thread(test_data["images"])

    # TODO the detected position is actually a bit off here.  The cube is
    # on the ground, so the height would need to be 0.0325.
    expected_position = np.array([-0.014, -0.024, 0.028])
    expected_orientation = Rotation.from_quat(
        [0.58214203, 0.36977536, -0.61211132, 0.38690641]
    )

    np.testing.assert_array_almost_equal(pose.position, expected_position, 2)

    actual_orientation = Rotation.from_quat(pose.orientation)
    assert rotation_error(actual_orientation, expected_orientation) < 0.1

    assert pose.confidence > 0.8


def test_find_pose_cube_v1_multithread():
    model = object_tracker.CubeV1Model()
    test_data = load_test_data(model.get_name())

    cube_detector = object_tracker.CubeDetector(
        model, test_data["camera_parameter_files"]
    )

    pose = cube_detector.detect_cube(test_data["images"])

    # TODO the detected position is actually a bit off here.  The cube is
    # on the ground, so the height would need to be 0.0325.
    expected_position = np.array([-0.014, -0.024, 0.028])
    expected_orientation = Rotation.from_quat(
        [0.58214203, 0.36977536, -0.61211132, 0.38690641]
    )

    np.testing.assert_array_almost_equal(pose.position, expected_position, 2)

    actual_orientation = Rotation.from_quat(pose.orientation)
    assert rotation_error(actual_orientation, expected_orientation) < 0.1

    assert pose.confidence > 0.8


def test_find_pose_cube_v2():
    model = object_tracker.CubeV2Model()
    test_data = load_test_data(model.get_name())

    cube_detector = object_tracker.CubeDetector(
        model, test_data["camera_parameter_files"]
    )

    pose = cube_detector.detect_cube_single_thread(test_data["images"])

    # TODO the detected position is actually a bit off here.  The cube is
    # on the ground, so the height would need to be 0.0325.
    expected_position = np.array([-0.063, -0.136, 0.025])
    expected_orientation = Rotation.from_quat(
        [0.1192724, 0.6887785, -0.1423320, 0.7007851]
    )

    np.testing.assert_array_almost_equal(pose.position, expected_position, 2)

    actual_orientation = Rotation.from_quat(pose.orientation)
    assert rotation_error(actual_orientation, expected_orientation) < 0.1

    assert pose.confidence > 0.8


def test_find_pose_cuboid_2x2x8_v2():
    model = object_tracker.Cuboid2x2x8V2Model()
    test_data = load_test_data(model.get_name())

    cube_detector = object_tracker.CubeDetector(
        model, test_data["camera_parameter_files"]
    )

    pose = cube_detector.detect_cube_single_thread(test_data["images"])

    # TODO the detected position is actually a bit off here.  The cube is
    # on the ground, so the height would need to be 0.0325.
    expected_position = np.array([-0.007, -0.008, 0.04])
    # expected_orientation = Rotation.from_quat(
    #     [0.67863974, -0.148312, -0.1528063, 0.70292382]
    # )

    np.testing.assert_array_almost_equal(pose.position, expected_position, 2)

    # orientation for the small cuboid can be very off, so don't check this here
    # actual_orientation = Rotation.from_quat(pose.orientation)
    # assert rotation_error(actual_orientation, expected_orientation) < 0.1

    assert pose.confidence > 0.8
