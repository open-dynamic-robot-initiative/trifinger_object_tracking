#ifndef XGBOOST_CLASSIFIER_H
#define XGBOOST_CLASSIFIER_H

#include <array>

constexpr int XGB_NUM_CLASSES = 7;
constexpr int XGB_NUM_FEATURES = 6;

std::array<float, XGB_NUM_CLASSES> xgb_classify(
    std::array<float, XGB_NUM_FEATURES> &sample);

#endif
