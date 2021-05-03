#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <trifinger_object_tracking/xgboost_classifier_single_color_rgb.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Wrong number of arguments. " << argc << std::endl;
        std::cout << "Usage: " << argv[0] << " path/to/image" << std::endl;
        return 1;
    }

    std::string image_path(argv[1]);
    std::cout << "load image " << image_path << std::endl;

    cv::Mat img = cv::imread(image_path);
    cv::Mat mask = trifinger_object_tracking::segment_image(img);

    // apply the mask on the image to get the segmented areas
    cv::Mat segmented;
    img.copyTo(segmented, mask);

    // concat images for nicer visualisation
    cv::Mat merged;
    cv::hconcat(img, segmented, merged);
    cv::imshow("Lightblue Segmenter", merged);
    cv::waitKey(0);

    return 0;
}
