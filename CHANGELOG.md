# Changelog

All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [Unreleased]
### Added
- Add method `get_debug_image` to `TriCameraObjectTrackerDriver` and a script
  that uses it to create debug images from the cameras (this makes diagnosing
  issues with the object detection easier).

### Removed
- The `ProgramOptions` class has been moved to its own package
  ([cli_utils](https://github.com/MPI-IS/cli_utils)).  Thus it is removed from
  this package and the one from cli_utils is used instead.

### Fixed
- pybind11 build error on Ubuntu 22.04


## [1.1.0] - 2022-07-29
### Added
- Parameter to `CubeVisualizer::draw_cube` to set opacity of the visualisation.
- `CubeVisualizer::draw_circle` to draw a circle at the objects position (e.g.
  for cases where the orientation does not matter).

## [1.0.0] - 2022-06-28
### Added
- Executable `benchmark_with_logfile` to benchmark the time needed for the
  object detection using a given camera log file.

### Changed
- Build models for all supported object types and select the type at run-time
  (instead of using a build-flag).  **For this the API of several classes had to
  be changed as the object model now needs to be passed as argument.**
- Update the colour segmentation model for cube v2.  The new model is trained on
  a larger dataset that contains some images of a dirty cube.  This should make
  the model more robust against dirt/dust on the cube.

### Fixed
- Set `filtered_object_pose` in observations when using the PyBullet camera
  driver.


## [0.2.0] - 2021.08.04

There is no changelog for this or earlier versions.


[Unreleased]: https://github.com/open-dynamic-robot-initiative/trifinger_object_tracking/compare/v1.1.0...HEAD
[1.1.0]: https://github.com/open-dynamic-robot-initiative/trifinger_object_tracking/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/open-dynamic-robot-initiative/trifinger_object_tracking/compare/v0.2.0...v1.0.0
[0.2.0]: https://github.com/open-dynamic-robot-initiative/trifinger_object_tracking/releases/tag/v0.2.0
