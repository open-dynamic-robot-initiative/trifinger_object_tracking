# %%
import os
import cv2 as cv
import pickle
import matplotlib.pyplot as plt
import ipdb
import numpy as np
import random
import itertools

import sys
import pandas as pd
from PIL import Image
import traceback


from scipy import ndimage


def show_image(image):
    image = Image.fromarray(image[:, :, ::-1], "RGB")
    image.show()


def load_images(path):

    image_names = [n for n in os.listdir(path) if ".png" in n]

    images = {}
    for image_name in image_names:
        image_path = os.path.join(path, image_name)
        image = cv.imread(image_path)  # BGR
        images[image_name] = image

    return images


# %%

if __name__ == "__main__":
    try:
        # %%
        # load data ----------------------------------------------------
        path = "./cube_image_set_p4_2020-09-16/"
        frame_folders = [
            f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f))
        ]

        data = []
        for frame_folder in frame_folders:
            frame_path = os.path.join(path, frame_folder)
            segment_names = [
                name for name in os.listdir(frame_path) if name[1].isdigit()
            ]

            if len(segment_names) == 0:
                continue

            images = load_images(frame_path)

            for camera_name in ["60", "180", "300"]:
                image = images["camera" + camera_name + ".png"]

                masks = {
                    key: images[key]
                    for key in images.keys()
                    if camera_name in key
                    and key[1].isdigit()
                    and key[0] != "n"
                }

                for key in masks:
                    binary_mask = np.sum(masks[key], axis=2) == 0
                    eroded_binary_mask = ndimage.binary_erosion(
                        binary_mask, structure=np.ones([10, 10])
                    )
                    image = eroded_binary_mask[..., None] * image

                cv.imwrite(
                    os.path.join(frame_path, "n" + camera_name + ".png"), image
                )
                print(os.path.join(frame_path, "n" + camera_name + ".png"))

    except:
        extype, value, tb = sys.exc_info()
        traceback.print_exc()
        ipdb.post_mortem(tb)
