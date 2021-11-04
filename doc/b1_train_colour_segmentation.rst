************************************************
Train Colour Segmentation for the Object Tracker
************************************************


This article describes all steps necessary to train a colour segmentation model
that can be used by the object tracker.  The procedure can be roughly summarised
as follows:

1. Record images and label them to create training data.
2. Train an xgboost model.
3. From the trained model, generate efficient C++ code and integrate it into the
   object tracker.


Generate a Dataset
==================

Collect Images
--------------

First of all, record some images of the object in the TriFinger platform.  First
run

::

    ros2 run trifinger_cameras record_image_dataset --no-downsample -o <output_directory>

Every time you press Enter, images of all three cameras are acquired and stored
together as a "snapshot" in the specified output directory.  So you can place
the object, press Enter to record images, move the object to a new position,
press Enter to record another image, and so on.

Make sure to take enough images such that all sides of the object are covered as
well as the whole workspace of the platform (as the lighting is different
depending on the position.  Also vary the pose of the manipulators for the same
reason.  While this is not so relevant for the colour segmentation, you may also
want to record images with varying amount of occlusion for later testing of the
actual tracking.

You will end up with the following file structure (each subdirectory of
``output_directory`` corresponding to one snapshot::

    output_directory
    ├── 0001
    │   ├── camera180.png
    │   ├── camera300.png
    │   └── camera60.png
    ├── 0002
    │   ├── camera180.png
    │   ├── camera300.png
    │   └── camera60.png
    ...


Label Colours in the Images
---------------------------

Now comes the annoying part:  To get a dataset for training the colour
segmentation model, the recorded images need to be labelled manually.

You don't need to label all snapshots of the dataset (the unlabelled ones will
be used for testing), however the ones that are labelled need to be complete
(i.e. add labels for all three images and for all colours that are visible).

Labels are given in the form of separate mask images, one per visible colour.
The masks should have the same size as the image and only consist of pure black
and white pixels (white indicating the pixels that belong to the corresponding
colour).  As they are binary, they can be stored as single-channel images to
save disk space but three-channel images work as well.

The mask images have to be in PNG format and be named with the pattern

::

    {colour_code}{camera_id}.png

The colour codes are:

* red: r
* green: g
* blue: b
* cyan: c
* magenta: m
* yellow: y

So for example the red mask for ``camera_180.png`` would be called ``r180.png``.

There is a plugin for Gimp in the ``gimp_plugin`` directory of this repository
which helps with creating the masks.  See the README there for more details.

When masking, rather be a bit conservative at the edges.  It is better to
exclude a few good pixels at the edge than to include some bad pixels.

You only need to create a mask for colours that are visible in the images.  A
non-existent mask will be interpreted as "this colour does not appear in that
image".


Generate Background Labels
--------------------------

In the previous step only the labels for the colours were created.  For the
training, there also need to be masks for the background.  Those can be
generated automatically, though.  Simply run

::

    ros2 run trifinger_object_tracking create_background_data <dataset_dir>

The script will automatically go through the dataset and create background masks
(using the colour code "n") based on the colour masks.


Verify the Labels
-----------------

Before starting the training, you should verify that all masks are correct (i.e.
do not include pixels of a wrong colour).

::

    ros2 run trifinger_object_tracking view_masks <dataset_dir> <colour_code>

It will display all masks of the specified colour one after another (press any
key to go to the next one).  Do this for all colours and, most importantly, for
the background masks.

In rare cases, the Gimp plugin seems to not work correctly, resulting in a mask
where the white is not actually true white (i.e. only something like 250 instead
of 255).  In this case, the background mask will not be correct.  If you see
such a case, fix the corresponding colour masks and run the background mask
creation again.


Train the Segmentation Model
============================

The colour segmentation is done using a xgboost model which is trained in Python
and then converted to static C++ code.

First, train the model::

    ros2 run trifinger_object_tracking train_xgb_tree --train \
        --image-dir <dataset_dir>
        [--output-dir <test_output_dir>]

The ``--output-dir`` argument is optional.  If set, the newly trained model will
be applied to the unlabelled images in the dataset and the results will be
written to the given directory.  You can also run this step separately after the
training by running the above command without the ``--train`` flag.

The following files will be created in the current working directory:

- data.pkl: The data set generated from the images (this is only needed for
  training/testing with this script and does not need to be kept permanently).
- xgb_model.bin: The trained model in binary format
- xgb_model.bin_dump.txt: A text file with a dump of the model


Generate C++ Code
-----------------

While xgboost has a C++ API, this is cumbersome to use and not well
documentation.  Instead, we can generate static C++ code from the trained model
which should also give a significant performance boost.

For this, first a proper txt dump of the model needs to be created.  We are
already getting such a dump from the training script
(``xgb_model.bin_dump.txt``), however, this file contains the actual feature
names, which is nice in general but unfortunately the "dump to C++" conversion
will not work with this.  Therefore we need to create a new dump without the
feature names::

    ros2 run trifinger_object_tracking get_xgb_dump xgb_model.bin xgb_model_dump.txt

The dump produced this way does not know about the original feature names but
just calls them "f0", "f1", ...
You may want to keep the original dump with the feature names as well, so you
can compare which feature "fx" corresponds to.

From this second dump, we can now generate the C++ code.  This
can be easily converted to a C++ file with static if/else statements using
`xgb2cpp <https://github.com/popcorn/xgb2cpp>`_ (in case the original repo
disappears, we have a `backup fork <https://github.com/luator/xgb2cpp>`_)::

    python generate_cpp_code.py --num_classes 7 --xgb_dump xgb_model_dump.txt

This creates a file ``xgboost_classifier.cpp`` in the current working directory.


Integrate the model in the object tracker
-----------------------------------------

Copy the generated ``xgboost_classifier.cpp`` to ``src/object_name/`` of the
trifinger_object_tracking package.  Then open it and apply the following
changes (they make the code more efficient):

1. Adjust the header include to

   .. code-block:: c++

       #include <trifinger_object_tracking/xgboost_classifier.h>

2. Replace ``std::vector`` with ``std::array`` (for better performance):

   .. code-block:: c++

       std::array<float, XGB_NUM_CLASSES> xgb_classify(std::array<float, XGB_NUM_FEATURES> &sample) {

         std::array<float, XGB_NUM_CLASSES> sum;
         sum.fill(0.0);

Finally update the ``CMakeLists.txt`` to use this file (currently done by
setting the ``cube_model_dir`` variable).
