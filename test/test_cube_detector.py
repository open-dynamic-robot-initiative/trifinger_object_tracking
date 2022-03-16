import pathlib

import pytest
import cv2
import numpy as np

import trifinger_object_tracking.py_object_tracker as object_tracker


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


def test_find_pose_cube_v1():
    model = object_tracker.CubeV1Model()
    test_data = load_test_data(model.get_name())

    cube_detector = object_tracker.CubeDetector(
        model, test_data["camera_parameter_files"]
    )

    # FIXME add tests for multi-thread version
    pose = cube_detector.detect_cube_single_thread(test_data["images"])

    # TODO the detected position is actually a bit off here.  The cube is
    # on the ground, so the height would need to be 0.0325.
    actual_position = np.array([-0.014, -0.024, 0.028])
    actual_orientation = np.array(
        [0.58214203, 0.36977536, -0.61211132, 0.38690641]
    )

    np.testing.assert_array_almost_equal(pose.position, actual_position, 2)

    # TODO What is a good way to compare orientation vectors?
    # np.testing.assert_array_almost_equal(pose.orientation, actual_orientation, 2)

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
    actual_position = np.array([-0.063, -0.136, 0.025])
    actual_orientation = np.array(
        [0.1192724, 0.6887785, -0.1423320, 0.7007851]
    )

    np.testing.assert_array_almost_equal(pose.position, actual_position, 2)

    # TODO What is a good way to compare orientation vectors?
    # np.testing.assert_array_almost_equal(pose.orientation, actual_orientation, 2)

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
    actual_position = np.array([-0.007, -0.008, 0.04])
    actual_orientation = np.array(
        [0.67863974, -0.148312, -0.1528063, 0.70292382]
    )

    np.testing.assert_array_almost_equal(pose.position, actual_position, 2)

    # TODO What is a good way to compare orientation vectors?
    # np.testing.assert_array_almost_equal(pose.orientation, actual_orientation, 2)

    assert pose.confidence > 0.8
