/**
 * @file
 * @brief Python bindings for the lightblue colour segmentation
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 */
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <pybind11_opencv/cvbind.hpp>

#include <trifinger_object_tracking/xgboost_classifier_single_color_rgb.h>

PYBIND11_MODULE(py_lightblue_segmenter, m)
{
    m.def("segment_image",
          &trifinger_object_tracking::segment_image,
          pybind11::arg("image_bgr"),
          R"XXX(
          Segment the lightblue areas of the given image.

          Args:
              image_bgr: The image in BGR colour space.

          Returns:
              The segmentation mask.
)XXX",
          pybind11::call_guard<pybind11::gil_scoped_release>());
}
