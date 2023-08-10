# %%
import ipdb
import sys
import traceback
import numpy as np
import cv2 as cv2
from PIL import Image


def bgr2rgb(bgr_image):
    return bgr_image[:, :, ::-1]


def downsample_raw_image(image):
    # ported from C++ code
    # Downsample resolution by factor 2.  We are operating on the raw
    # image here, so we need to be careful to preserve the Bayer
    # pattern. This is done by iterating in steps of 4 over the
    # original image, keeping the first two rows/columns and discarding
    # the second two.
    rows = image.shape[0] // 2
    cols = image.shape[1] // 2

    downsampled = np.zeros((rows, cols), dtype=np.uint8)

    for r in range(rows)[::2]:
        for c in range(cols)[::2]:
            r2 = r * 2
            c2 = c * 2

            downsampled[r, c] = image[r2, c2]
            downsampled[r + 1, c] = image[r2 + 1, c2]
            downsampled[r, c + 1] = image[r2, c2 + 1]
            downsampled[r + 1, c + 1] = image[r2 + 1, c2 + 1]

    return downsampled


def rbg_to_bayer_bg(image: np.ndarray) -> np.ndarray:
    # copied
    """Convert an rgb image to a BG Bayer pattern.

    This can be used to generate simulated raw camera data in Bayer format.
    Note that there will be some loss in image quality.  It is mostly meant for
    testing the full software pipeline with the same conditions as on the real
    robot.  It is not optimized of realistic images.

    Args:
        image: RGB image.

    Returns:
        Bayer pattern based on the input image.  Height and width are the same
        as of the input image.  The image can be converted using OpenCV's
        `COLOR_BAYER_BG2*`.
    """
    # there is only one channel but it still needs the third dimension, so that
    # the conversion to a cv2.Mat in C++ is easier

    bayer_img = np.zeros((image.shape[0], image.shape[1], 1), dtype=np.uint8)

    # channel names, assuming input is RGB
    CHANNEL_RED = 0
    CHANNEL_GREEN = 1
    CHANNEL_BLUE = 2

    # channel map to get the following pattern (called "BG" in OpenCV):
    #
    #   R G
    #   G B
    #
    channel_map = {
        (0, 0): CHANNEL_RED,
        (1, 0): CHANNEL_GREEN,
        (0, 1): CHANNEL_GREEN,
        (1, 1): CHANNEL_BLUE,
    }

    for r in range(image.shape[0]):
        for c in range(image.shape[1]):
            channel = channel_map[(r % 2, c % 2)]
            bayer_img[r, c] = image[r, c, channel]

    return bayer_img


def toy_example():
    size = 8
    bayer = np.arange(size**2, dtype=np.uint8).reshape([size, size, 1])
    bayer = np.random.randint(255, size=[size, size], dtype=np.uint8)
    image = cv2.cvtColor(bayer, cv2.COLOR_BAYER_BG2BGR)
    bayer = bayer.squeeze()

    for i, color in enumerate(["b", "g", "r"]):
        print("bayer")
        print(bayer)
        channel = image[:, :, i]
        print(color + "------------------------------------------------")
        print(channel)
        print("diff")
        diff = np.array(channel, dtype=np.int) - bayer
        print(diff)


def downsample_bayer_debayer(*, image, interpolation):
    num_rows = image.shape[0]
    num_cols = image.shape[1]

    downsampled = cv2.resize(
        image, (num_rows // 2, num_cols // 2), interpolation=interpolation
    )

    downsample_bayered = rbg_to_bayer_bg(bgr2rgb(downsampled))
    downsample_bayer_debayered = cv2.cvtColor(
        downsample_bayered, cv2.COLOR_BAYER_BG2BGR
    )

    return downsample_bayer_debayered


def bayer_downsample_debayer(image):
    bayered = rbg_to_bayer_bg(bgr2rgb(image))
    bayer_downsampled = downsample_raw_image(bayered)
    bayer_downsample_debayered = cv2.cvtColor(
        bayer_downsampled, cv2.COLOR_BAYER_BG2BGR
    )

    return bayer_downsample_debayered


def compare_bayer_downsampling():
    # linear seems one of the best
    # COLOR_BAYER_BG2
    path = "./cube_v2__pro4__2020-09-20/0015/camera180.png"
    # path = './lena.png'

    original = cv2.imread(path)

    downsample_bayer_debayered = downsample_bayer_debayer(
        image=original, interpolation=cv2.INTER_NEAREST
    )

    bayer_downsample_debayered = bayer_downsample_debayer(original)
    cv2.imshow("get fucked", downsample_bayer_debayered)
    cv2.imshow("downsample_bayer_debayered", downsample_bayer_debayered)
    cv2.imshow("bayer_downsample_debayered", bayer_downsample_debayered)

    cv2.waitKey(0)
    cv2.destroyAllWindows()


interpolation_methods = [
    cv2.INTER_NEAREST,
    cv2.INTER_LINEAR,
    cv2.INTER_CUBIC,
    cv2.INTER_AREA,
    #  cv2.INTER_LANCZOS4,
    cv2.INTER_LINEAR_EXACT,
    #  cv2.INTER_NEAREST_EXACT,
    #  cv2.INTER_MAX,
    #  cv2.WARP_FILL_OUTLIERS,
]


def compare_interpolation():
    # COLOR_BAYER_BG2
    path = "./cube_v2__pro4__2020-09-20/0015/camera180.png"
    # path = './lena.png'

    original = cv2.imread(path)
    cv2.imshow("get fucked", original)

    for interpolation in interpolation_methods:
        result = downsample_bayer_debayer(
            image=original, interpolation=interpolation
        )

        cv2.imshow("interpolation: " + str(interpolation), result)

    cv2.waitKey(0)
    cv2.destroyAllWindows()


def compare_filters():
    # median before downsampling seems to be best

    path = "./cube_v2__pro4__2020-09-20/0015/camera180.png"

    original = cv2.imread(path)
    cv2.imshow("get fucked", original)

    # result without filtering
    result = downsample_bayer_debayer(
        image=original, interpolation=cv2.INTER_LINEAR
    )

    # filtering before downsampling
    result_median_before = downsample_bayer_debayer(
        image=cv2.medianBlur(original, 3), interpolation=cv2.INTER_LINEAR
    )
    result_gaussian_before = downsample_bayer_debayer(
        image=cv2.GaussianBlur(original, (3, 3), 0),
        interpolation=cv2.INTER_LINEAR,
    )

    # filtering at the end
    result_median_end = cv2.medianBlur(result, 3)

    cv2.imshow("result", result)
    cv2.imshow("result_median_end", result_median_end)

    cv2.imshow("result_median_before", result_median_before)
    cv2.imshow("result_gaussian_before", result_gaussian_before)

    cv2.waitKey(0)
    cv2.destroyAllWindows()


# %%
if __name__ == "__main__":
    try:
        compare_filters()

    except:
        extype, value, tb = sys.exc_info()
        traceback.print_exc()
        ipdb.post_mortem(tb)
# %%
