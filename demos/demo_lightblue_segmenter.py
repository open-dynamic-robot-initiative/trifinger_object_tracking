#!/usr/bin/env python3
"""Run the 'lightblue' segmenter on single images"""
import argparse
import sys

import cv2
import numpy as np
from trifinger_object_tracking.py_lightblue_segmenter import segment_image


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_image")
    args = parser.parse_args()

    # load the image
    img = cv2.imread(args.input_image)

    # get the segmentation mask
    mask = segment_image(img)

    # for nicer visualisation get the masked area from the original image
    segmented = cv2.bitwise_and(img, img, mask=mask)

    # stack together for visualisation
    both = np.hstack([img, segmented])
    cv2.imshow("Lightblue Segmenter", both)
    cv2.waitKey()

    return 0


if __name__ == "__main__":
    sys.exit(main())
