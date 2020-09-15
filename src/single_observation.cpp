/**
 * @brief Run pose detection on image files of one camera observation.
 *
 * Load images of the three cameras from files "camera{60,180,300}.png", run the
 * pose detection on them and visualize the result.
 */
#include <iostream>
#include <trifinger_object_tracking/cv_sub_images.hpp>
#include <trifinger_object_tracking/image.hpp>
#include <trifinger_object_tracking/pose.hpp>


/**
 * @brief Load images camera{60,180,300}.png from the given directory.
 *
 * @param directory Directory containing the image files.
 *
 * @return Array with images [camera60.png, camera180.png, camera300.png]
 */
std::array<cv::Mat, 3> load_images(const std::string &directory)
{
    cv::Mat image60 = cv::imread(directory + "/camera60.png");
    cv::Mat image180 = cv::imread(directory + "/camera180.png");
    cv::Mat image300 = cv::imread(directory + "/camera300.png");

    if (image60.empty() || image180.empty() || image300.empty())
    {
        throw std::runtime_error("Failed to load image.");
    }

    return {image60, image180, image300};
}


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Invalid number of arguments." << std::endl;
        std::cout << "Usage: " << argv[0] << " <path to images>" << std::endl;
        return 1;
    }

    const std::string data_dir = argv[1];

    trifinger_object_tracking::CvSubImages subplot(cv::Size(720, 540), 3, 5);
    cv::namedWindow("debug", cv::WINDOW_NORMAL);
    cv::resizeWindow(
        "debug", subplot.get_image().cols, subplot.get_image().rows);

    auto frames = load_images(data_dir);

    std::vector<trifinger_object_tracking::Image> images;

    int i = 0;
    for (auto image : frames)
    {
        // TODO: move this processing to somewhere else!
        cv::fastNlMeansDenoisingColored(image, image, 10, 10, 7, 21);
        cv::GaussianBlur(image, image, cv::Size(5, 5), 0);
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

        // TODO: line detector class should not take a fixed image in c'tor
        trifinger_object_tracking::Image line_detector(image.clone());

        // FIXME name: "start" thread? This suggests that it does not wait until
        // it is finished, but I think it is doing this.
        line_detector.startSingleThread();

        images.push_back(line_detector);

        subplot.set_subimg(line_detector.get_image(), i, 0);
        subplot.set_subimg(line_detector.get_segmented_image(), i, 1);
        subplot.set_subimg(
            line_detector.get_segmented_image_wout_outliers(), i, 2);
        subplot.set_subimg(line_detector.get_image_lines(), i, 3);

        i++;
    }

    // TODO: do not pass the full line detection class but only what is really
    // needed
    trifinger_object_tracking::Pose pose(images);
    pose.find_pose();


    // visualize the detected pose
    // TODO: make this a method of pose detector?
    for (int i = 0; i < images.size(); i++)
    {
        std::vector<cv::Point2f> imgpoints = pose.projected_points_[i];
        cv::Mat poseimg = images[i].get_image().clone();
        // draw the cube edges in the image
        for (auto &it : images[i].object_model_)
        {
            cv::Point p1, p2;
            p1.x = imgpoints[it.second.first].x;
            p1.y = imgpoints[it.second.first].y;
            p2.x = imgpoints[it.second.second].x;
            p2.y = imgpoints[it.second.second].y;

            cv::line(poseimg, p1, p2, cv::Scalar(255, 100, 0), 2);
        }
        subplot.set_subimg(poseimg, i, 4);
    }

    cv::Mat debug_img = subplot.get_image();

    cv::cvtColor(debug_img, debug_img, cv::COLOR_RGB2BGR);

    // scale down debug image by factor 2, otherwise it is too big
    cv::Mat rescaled_debug_img;
    cv::resize(debug_img,
               rescaled_debug_img,
               cv::Size(debug_img.cols / 2, debug_img.rows / 2));

    cv::imshow("debug", rescaled_debug_img);
    char key = cv::waitKey(0);

    return 0;
}

