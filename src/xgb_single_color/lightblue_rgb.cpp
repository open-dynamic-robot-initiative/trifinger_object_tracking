#include <trifinger_object_tracking/xgboost_classifier_single_color_rgb.h>

namespace trifinger_object_tracking
{
std::array<float, XGB_NUM_CLASSES> xgb_classify(
    const std::array<float, XGB_NUM_FEATURES> &sample)
{
    std::array<float, XGB_NUM_CLASSES> sum;
    sum.fill(0.0);

    if (sample[2] < 23.5)
    {
        if (sample[0] < 52)
        {
            sum[0] += -1.99386501;
        }
        else
        {
            if (sample[0] < 63.5)
            {
                sum[0] += 0.5;
            }
            else
            {
                sum[0] += 1.99344265;
            }
        }
    }
    else
    {
        if (sample[0] < 199.5)
        {
            if (sample[2] < 33.5)
            {
                if (sample[0] < 63.5)
                {
                    sum[0] += -1.99145293;
                }
                else
                {
                    if (sample[0] < 69.5)
                    {
                        sum[0] += 0.5;
                    }
                    else
                    {
                        sum[0] += 1.9375;
                    }
                }
            }
            else
            {
                if (sample[2] < 64.5)
                {
                    if (sample[0] < 84.5)
                    {
                        if (sample[0] < 81.5)
                        {
                            sum[0] += -1.99540007;
                        }
                        else
                        {
                            sum[0] += -0.571428597;
                        }
                    }
                    else
                    {
                        if (sample[0] < 93.5)
                        {
                            sum[0] += 0.923076928;
                        }
                        else
                        {
                            sum[0] += 1.95375717;
                        }
                    }
                }
                else
                {
                    sum[0] += -1.99684238;
                }
            }
        }
        else
        {
            if (sample[1] < 177)
            {
                sum[0] += 1.954023;
            }
            else
            {
                sum[0] += -1.99136066;
            }
        }
    }

    return sum;
}

}  // namespace trifinger_object_tracking
