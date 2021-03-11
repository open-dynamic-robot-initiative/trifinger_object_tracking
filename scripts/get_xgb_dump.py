#!/usr/bin/env python3
"Create a text dump from a binary xgboost model"
import argparse
import xgboost


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("in_model", type=str, help="Path to the model file")
    parser.add_argument(
        "out_dump", type=str, help="Path to where the dump will be saved"
    )
    args = parser.parse_args()

    # Simply load the binary model and dump it to a text file.  At this point,
    # there is no information about the original feature names anymore, so they
    # will simply be numbered.
    model = xgboost.XGBClassifier()
    model.load_model(args.in_model)
    model.get_booster().dump_model(args.out_dump)
