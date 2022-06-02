GIMP Plug-in for Creating Masks
===============================

This is a GIMP plug-in that assists in creating masks from selections.

Installation
------------

Copy `create_mask.py` to one of the "plug-in folders" (something like
`~/.config/GIMP/2.10/plug-ins`).  The folders are listed (and can be edited) in
the GIMP settings:

    Edit > Preferences > Folders > Plug-ins

Then restart GIMP for the plug-in to be loaded.

You should now have the option "MaskImage" at the bottom of the "File" menu.


Usage
-----

Make sure the foreground colour is set to pure black and the background colour
to pure white.

To create a mask, first select the desired area in the image.  **Make sure to
disable antialiasing and feathering of the selection tool to get sharp selection
edges!**

Select an area in the image and run this option.  It will convert the image into
a 1-bit black and white image based on the selection.  You can directly save
this (Ctrl+Shift+E) and then undo with Ctrl+Z to get back to the original
image.

You can re-apply the last plug-in with Ctrl+F, so you don't need to select it
from the menu every time.


Trouble Shooting
----------------

### Plug-in not showing up or not working

If the "MaskImage" entry is not showing up in the File menu or it is not working
in some other way, start GIMP from the terminal and check the output there.
It may print some error messages that help understanding the cause of the
problem.


### ImportError: No module named gimpfu

Probably caused by Python version issues on newer systems (GIMP 2.x still
depends on Python 2).  Can be solved by installing GIMP via flatpak.

See https://stackoverflow.com/q/69962119
