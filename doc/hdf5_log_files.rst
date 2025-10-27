**********************
Camera Log HDF5 Format
**********************

The executable ``tricamera_log_to_hdf5`` can be used to convert existing logs of
TriCamera data with object pose information (as produced by this package) to the more
portable HDF5 format.

The data format is the same as described in :doc:`trifinger_cameras:doc/hdf5_log_files`
but extended by the following datasets:

- ``object_pose/has_pose``: Boolean indicating whether the object was found at all.  If
  `false`, `confidence` is expected to be zero and values of `position` and
  `orientation` are meaningless.
- ``object_pose/position``: The estimated x, y, z-position of the object w.r.t. the
  world frame.  Dimensions: ``(n_observations, 3)``.
- ``object_pose/orientation``: The estimated orientation of the object w.r.t. the world
  frame.  Given as quaternions ``(x, y, z, w)``.  Dimensions: ``(n_observations, 4)``.
- ``object_pose/confidence``: The confidence measure of the pose estimation.
  Dimensions: ``(n_observations, 1)``.
