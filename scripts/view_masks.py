#!/usr/bin/env python3
"""View masks for visual verification.

Loads masks of the specified colours and shows them one by one.

Press any key != "q" to go to next image.  Press "q" to quit.
"""
import argparse
import pathlib

import cv2


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("data_dir")
    parser.add_argument("color_code", choices=list("rgbcmyn"))
    args = parser.parse_args()

    data_dir = pathlib.Path(args.data_dir)

    images = []
    for cam_id in ("60", "180", "300"):
        for mask in data_dir.glob("*/" + args.color_code + cam_id + ".png"):
            orig_file = mask.with_name("camera" + cam_id + ".png")
            images.append((mask, orig_file))

    for mask_file, orig_file in images:
        print(mask_file)
        mask_img = cv2.imread(str(mask_file))

        # If it is the background mask, show it as is.  Otherwise get the
        # corresponding part from the original image (the mask itself is just
        # black/white).
        if args.color_code != "n":
            orig_img = cv2.imread(str(orig_file))
            mask_img = cv2.bitwise_and(orig_img, mask_img)

        cv2.imshow("Image", mask_img)
        key = cv2.waitKey(0)
        if chr(key) == "q":
            return


if __name__ == "__main__":
    main()
