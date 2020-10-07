#!/usr/bin/env python3
"""Load file with numbers and compute mean, min, max, etc."""
import argparse
import numpy as np


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("filename")
    args = parser.parse_args()

    data = np.loadtxt(args.filename)

    print("n:", len(data))
    print("mean:", data.mean())
    print("std:", data.std())
    print("min:", data.min())
    print("max:", data.max())


if __name__ == "__main__":
    main()
