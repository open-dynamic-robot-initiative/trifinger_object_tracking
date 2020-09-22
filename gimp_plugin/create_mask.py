#!/usr/bin/env python
from gimpfu import *


def create_mask_from_selection(image, drawable):
    #image = gimp.image_list()[0]
    #drawable = pdb.gimp_image_get_active_drawable(image)

    pdb.gimp_image_undo_group_start(image)

    pdb.gimp_edit_bucket_fill(drawable, 1, 0, 100., 255., 0, 0.0, 0.0)
    pdb.gimp_selection_invert(image)
    pdb.gimp_edit_bucket_fill(drawable, 0, 0, 100., 255., 0, 0.0, 0.0)

    # change to binary pallete
    pdb.gimp_convert_indexed(image, NO_DITHER, 0, 2, False, True, "")

    pdb.gimp_image_undo_group_end(image)


register(
    "create_mask_from_selection",
    "Create mask image from selection",
    "Create a black/white mask from the current selection",
    "Felix Widmaier",
    "Felix Widmaier",
    "2020",
    "<Image>/File/MaskImage",
    "RGB*, GRAY*",
    [],
    [],
    create_mask_from_selection,
)

main()
