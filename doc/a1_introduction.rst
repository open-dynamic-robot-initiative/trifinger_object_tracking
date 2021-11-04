************
Introduction
************

The ``trifinger_object_tracking`` package provides pose estimation of a
multi-colours cuboid using multiple RGB cameras.  It is used in the TriFinger
robot platforms but should also work in other situations.  In the following, we
give a high level overview about how the detection is working.


Requirements
============

**Object:**  The object needs to be a cuboid with a different colour on each
side.  Further none of the used colours should appear anywhere else in the
scene.

**Cameras:**  Multiple RGB cameras need to provide synchronised images of the
object from different points of view (as wide-spread as possible).  We are
working with three cameras but the method should in principle also work with
more cameras.
Intrinsic and extrinsic calibration parameters of all cameras need to be
provided.

**Colour Segmentation:**  A colour segmentation model needs to be provided for
the colours of the used object.  See :doc:`b1_train_colour_segmentation`.


Method
======

The general procedure works as follows:

1. Segment all input images based on pixel-wise colour.
2. Run a genetic search algorithm which for each sample projects the cube into
   the images and checks how well it matches with the segmented colours.

Note that no actual tracking is done, i.e. no information from previous frames
is used.


Colour Segmentation
-------------------

The colour segmentation is done using a pixel-wise XGBoost classifier.  The
model is trained in Python using manually labelled images and then converted to
static C++ code (basically a big ``if/else`` tree) for best performance.
See :doc:`b1_train_colour_segmentation` for more details on how to train the
model.

The input of the model are the RGB and HSV values of a single pixel, the output
is a label denoting either one of the six colours or the background.  From this
a binary segmentation mask is generated for each colour.

To reduce noise, the image is first blurred a bit before applying the
classifier and afterwards morpholocial opening is applied on the segmentation
masks to remove single-pixel noise.

Since in one image at most three sides of the cuboid can be visible at once, the
colours are ordered by the number of pixels and only the three most dominant
colours are considered in the following pose estimation step.


Pose Estimation
---------------

The basic idea is to find a pose that, when projecting the cube into the images,
matches best with the segmented pixels.

To speed up the optimisation, we are not using all the segmented pixels, though,
but sample a fixed number, where the number of pixels drawn from each mask is
chosen proportionally to the size of the mask.

The optimisation is done using the `*differential evolution* algorithm of
OptimLib <https://optimlib.readthedocs.io/en/latest/api/de.html>`_.


Cost Function
~~~~~~~~~~~~~

To compute the cost of a given pose sample is a weighted sum of two parts:

1. **Pixel distance cost:**
   The object is projected into each of the given images.  For each sampled mask
   pixel it is checked if it lies inside the face polygon of the corresponding
   object face.  If not the square root of its distance to the polygon is
   computed and added to the cost.

2. **Face visibility cost:**
   For each detected colour it is checked if in the given pose the corresponding
   face of the object is actually visible in to the camera.  The idea behind
   this is that if there are blue pixels in the image but with the estimated
   pose the blue side of the object is facing away from the camera, this
   estimate cannot be correct.

   If the face is visible to the camera, the "visibility cost" is zero,
   otherwise it is set to value proportional to the number of segmented pixels
   of that colour and how much the face is rotated away from the camera.
