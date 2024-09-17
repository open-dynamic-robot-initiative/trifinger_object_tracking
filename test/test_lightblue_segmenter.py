"""Test the "lightblue segmenter" via its Python bindings."""

import os
import pathlib

import pytest
import numpy as np
import cv2

from trifinger_object_tracking.py_lightblue_segmenter import segment_image
from trifinger_cameras.utils import convert_image


@pytest.fixture
def test_data_dir() -> pathlib.PurePath:
    this_file = pathlib.PurePath(__file__)
    test_data_dir = this_file.parent / this_file.stem
    return test_data_dir


@pytest.fixture
def test_image_path(test_data_dir) -> pathlib.PurePath:
    return test_data_dir / "test_image.png"


@pytest.fixture
def test_mask_path(test_data_dir) -> pathlib.PurePath:
    return test_data_dir / "test_mask.png"


def test_segment_image(test_image_path, test_mask_path):
    img = cv2.imread(os.fspath(test_image_path))
    true_mask = cv2.imread(os.fspath(test_mask_path), cv2.IMREAD_GRAYSCALE)

    mask = segment_image(img)

    # Check 1: There should be no segmented pixels outside the true mask (add
    # some tolerance to allow for a little bit of noise).
    false_positives = cv2.bitwise_and(mask, cv2.bitwise_not(true_mask))
    assert np.count_nonzero(false_positives) < 100

    # Check 2: Pixels inside the true mask should mostly be segmented.  Add
    # some generous tolerance and require only 70% to be segmented.
    true_positives = cv2.bitwise_and(mask, true_mask)
    num_true_positives = np.count_nonzero(true_positives)
    num_true_mask = np.count_nonzero(true_mask)
    assert num_true_positives > 0.7 * num_true_mask


def test_error_on_single_channel_input(test_image_path):
    # The user may accidentally input the raw single-channel image (i.e. not
    # debayered).  In this case a clear error should be thrown.

    # simply load the test image as greyscale to get a single-channel image
    raw_img = cv2.imread(os.fspath(test_image_path), cv2.IMREAD_GRAYSCALE)

    with pytest.raises(Exception):
        segment_image(raw_img)

    # after converting it, there should be no error
    img = convert_image(raw_img)
    try:
        segment_image(img)
    except Exception:
        pytest.fail("segment_image failed on valid input image.")
