#!/usr/bin/env python3
"""Extract images from a TriCameraObjectObservations log file."""

import trifinger_object_tracking.py_tricamera_types as tricamera
from trifinger_cameras.tools.tricamera_log_extract import tricamera_log_extract

if __name__ == "__main__":
    tricamera_log_extract(tricamera.LogReader)
