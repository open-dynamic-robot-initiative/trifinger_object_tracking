#!/usr/bin/env python3
"""Train an XGBoost model for colour segmentation of images.

Segmentation of up to six colours is supported.  Fewer number of colours are
possible simply by providing training data for only a subset of the colours.
"""
import argparse
import pathlib
import random
import sys
import time

import cv2 as cv
import numpy as np
import pandas as pd
from xgboost import XGBClassifier

from matplotlib import colors

filename2class = {}
filename2class["n"] = "background"
filename2class["y"] = "yellow"
filename2class["r"] = "red"
filename2class["m"] = "magenta"
filename2class["b"] = "blue"
filename2class["c"] = "cyan"
filename2class["g"] = "green"


def class2bgr(pixel_class):
    if pixel_class == "background":
        color = "black"
    else:
        color = pixel_class
    rgb = np.array(colors.to_rgba(color)[:3])
    rgb = (rgb * 255).astype("uint8")

    return rgb[::-1]


def compute_identical_fraction(a, b):
    assert a.shape == b.shape

    identical_fraction = (a == b).sum() / a.size
    return identical_fraction


def create_channels(image_bgr):
    conversions = {
        "hsv": cv.COLOR_BGR2HSV,
        # "xyz": cv.COLOR_BGR2XYZ,
        # "LAB": cv.COLOR_BGR2Lab,
        # "LUV": cv.COLOR_BGR2Luv,
    }

    channels = {"bgr"[i]: image_bgr[:, :, i] for i in range(3)}

    for key in conversions:
        image = cv.cvtColor(image_bgr, conversions[key])
        new_channels = {key[i]: image[:, :, i] for i in range(len(key))}

        channels = {**channels, **new_channels}
    return channels


def create_features(image_bgr, flatten=False):

    image_bgr = cv.medianBlur(image_bgr, 7)
    channels = create_channels(image_bgr=image_bgr)

    if flatten:
        channels = {key: channels[key].flatten() for key in channels}

    return channels, image_bgr.shape[:2]


def load_segment(path: pathlib.Path, name: str) -> pd.DataFrame:
    image = cv.imread(str(path / ("camera" + name[1:])))
    features, shape = create_features(image_bgr=image, flatten=True)
    data = pd.DataFrame(features)

    mask = cv.imread(str(path / name))
    mask = mask.sum(axis=2) != 0
    mask = mask.flatten()
    data = data[mask]

    data["class"] = filename2class[name[0]]

    return data


def balance_classes(data, background_ratio, random_state):
    foreground = data[data["class"] != "background"]

    min_class_size = foreground["class"].value_counts().min()
    foreground = foreground.groupby("class").apply(
        lambda d: d.sample(min_class_size, random_state=random_state)
    )
    foreground = foreground.reset_index(drop=True)

    background = data[data["class"] == "background"]
    n_background_points = int(background_ratio * foreground.shape[0])
    background = background.sample(
        n_background_points, random_state=random_state
    )

    return pd.concat([foreground, background])


def get_subdirectories(input_path: pathlib.Path):
    return [f for f in input_path.iterdir() if f.is_dir()]


def load_images_and_create_data(
    input_path: pathlib.Path, output_filename: str
):
    # go through the folders and load all the annotated images
    # then compute features and create a pandas frame

    print("loading images")

    data = []
    for frame_folder in get_subdirectories(input_path):
        segment_names = [
            f.name
            for f in frame_folder.iterdir()
            if f.is_file() and f.name[1].isdigit()
        ]

        for segment_name in segment_names:
            segment_data = load_segment(path=frame_folder, name=segment_name)
            segment_data["frame"] = frame_folder.name
            data.append(segment_data)

    pd_data = pd.concat(data, axis="index")
    pd_data.to_pickle(output_filename)

    print("done loading images")


def prepare_data(
    data: pd.DataFrame, feature_names, train_fraction, background_ratio, seed
):
    # create training and test data from entire data frame

    # we split according to frames, not single pixels
    # to properly test generalization to other frames
    frames = sorted(list(set(data["frame"])))
    random.seed(seed)
    random.shuffle(frames)
    n_train_frames = int(len(frames) * train_fraction)
    train_frames = frames[:n_train_frames]
    test_frames = frames[n_train_frames:]

    train_set = data.loc[data["frame"].isin(train_frames)]
    train_set = balance_classes(
        data=train_set, background_ratio=background_ratio, random_state=seed
    )
    train_set = train_set.sample(frac=1, random_state=seed)

    test_set = data.loc[data["frame"].isin(test_frames)]
    test_set = balance_classes(
        data=test_set, background_ratio=background_ratio, random_state=seed
    )
    test_set = test_set.sample(frac=1, random_state=seed)

    target = "class"
    X_train = train_set[feature_names]
    y_train = train_set[target]
    X_test = test_set[feature_names]
    y_test = test_set[target]

    assert not set(train_set["frame"]).intersection(set(test_set["frame"]))
    print(train_set["class"].value_counts())
    print(test_set["class"].value_counts())

    return X_train, y_train, X_test, y_test


def fit_model(X_train, y_train, seed=42):

    model = XGBClassifier(
        learning_rate=1.0,
        n_estimators=1,  # only one tree
        n_jobs=8,
        max_depth=6,  # maximum tree depth
        random_state=seed,
    )
    model.fit(X_train, y_train)

    return model


def evaluate(model, X, y):
    # compute and print fraction of correct labels
    # also measure time

    print("success rate: ", compute_identical_fraction(model.predict(X), y))

    start = time.time()
    model.predict(X)
    end = time.time()
    print("n evaluations: ", X.shape[0])
    print("elapsed time: ", end - start)


def load_data_and_fit_model(input_filename, output_filename, feature_names):

    print("preparing training data")
    data = pd.read_pickle(input_filename)
    X_train, y_train, X_test, y_test = prepare_data(
        data=data,
        feature_names=feature_names,
        train_fraction=0.8,
        background_ratio=20,
        seed=22,
    )
    print("done preparing training data")

    print("fitting model")
    model = fit_model(X_train, y_train)
    model.save_model(output_filename)
    model.get_booster().dump_model(output_filename + "_dump.txt")
    print("done fitting model")

    print("test data ------------------------")
    evaluate(model, X_test, y_test)
    print("train data -----------------------")
    evaluate(model, X_train, y_train)


def load_model_and_generate_evaluation_images(
    model_filename,
    input_path: pathlib.Path,
    output_path: pathlib.Path,
    feature_names,
):
    model = XGBClassifier()
    model.load_model(model_filename)

    for frame_folder in sorted(get_subdirectories(input_path)):
        segment_names = [
            f.name
            for f in frame_folder.iterdir()
            if f.is_file() and f.name[1].isdigit()
        ]

        if len(segment_names) != 0:
            continue

        for camera_name in ["60", "180", "300"]:
            image_name = "camera" + camera_name + ".png"
            print(frame_folder / image_name)
            image_bgr = cv.imread(str(frame_folder / image_name))
            features, shape = create_features(
                image_bgr=image_bgr, flatten=True
            )

            X = pd.DataFrame(features)[feature_names]
            y = model.predict(X)

            segments = y.reshape(shape)

            segments_bgr = [class2bgr(idx) for idx in segments.flatten()]
            segments_bgr = np.array(segments_bgr).reshape(*shape, 3)

            path = output_path / frame_folder.name
            if not path.exists():
                path.mkdir(parents=True)

            image_and_segments_bgr = np.concatenate(
                [image_bgr, segments_bgr], axis=1
            )
            segments_filename = "camera" + camera_name + "_segments" + ".png"
            cv.imwrite(
                filename=str(path / segments_filename),
                img=image_and_segments_bgr,
            )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--train",
        action="store_true",
        help="Train a new model.",
    )
    parser.add_argument(
        "--image-dir",
        required=True,
        type=pathlib.Path,
        help="Directory containing the training data.",
    )
    parser.add_argument(
        "--output-dir",
        type=pathlib.Path,
        help="Output directory for test run.",
    )
    args = parser.parse_args()

    # probably we could get rid of some of these features without
    # losing much accuracy
    # feature_names = "rgb" + "hsv" + "hls" + "xyz" + "LAB" + "LUV"
    # feature_names = "rgb" + "hsv"
    # remove duplicates and sort for reproducibility
    # feature_names = sorted(list(set(feature_names)))

    feature_names = ["b", "g", "r", "h", "s", "v"]
    # feature_names = ["b", "g", "r"]

    if args.train:
        load_images_and_create_data(
            input_path=args.image_dir, output_filename="data.pkl"
        )

        load_data_and_fit_model(
            input_filename="data.pkl",
            output_filename="xgb_model.bin",
            feature_names=feature_names,
        )

    if args.output_dir:
        load_model_and_generate_evaluation_images(
            model_filename="xgb_model.bin",
            input_path=args.image_dir,
            output_path=args.output_dir,
            feature_names=feature_names,
        )

    return 0


if __name__ == "__main__":
    sys.exit(main())
