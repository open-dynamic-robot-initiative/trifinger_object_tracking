Gimp Plugin for creating masks
==============================

Copy `create_mask.py` to `~/.gimp-2.8/plug-ins` and restart gimp.

You should now have the option "MaskImage" at the bottom of the "File" menu.

To create a mask, first select the desired area in the image.  **Make sure to
disable antialiasing and feathering of the selection tool to get sharp selection
edges!**

Select an area in the image and run this option.  It will create the image into
a 1-bit black and white image based on the selection.  You can directly save
this (Ctrl+Shift+E) and then undo with Ctrl+Z to get back to the original
image.

You can re-apply the last plugin with Ctrl+F, so you don't need to select it
from the menu every time.
