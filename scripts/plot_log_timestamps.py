#!/usr/bin/python3
"""Plot time stamps of a object tracker log file.

Usage:

    plot_timestamps.py path/to/logfile.json

"""
import sys
import json
import matplotlib.pyplot as plt

if __name__ == "__main__":
    filename = sys.argv[1]
    with open(filename) as fh:
        log = json.load(fh)

    data = log["object_poses"]

    timestamps = [d["t"] for d in data]

    slope = (timestamps[-1] - timestamps[0]) / len(timestamps)
    print("Slope: {}".format(slope))

    plt.plot(timestamps)
    plt.show()
