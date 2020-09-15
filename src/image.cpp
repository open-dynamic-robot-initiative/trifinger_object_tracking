#include <chrono>
#include <iostream>
#include <thread>
#include <trifinger_object_tracking/image.hpp>
#include <typeinfo>

namespace trifinger_object_tracking
{
int output_percentage_classification = 1;

static float calculateAccuracyPercent(const cv::Mat &original,
                                      const cv::Mat &predicted)
{
    cv::Mat diff = original == predicted;
    return 100 * (float)countNonZero(original == predicted) / predicted.rows;
}

Image::Image(cv::Mat img, std::string model_directory)
    : image_(img), model_directory_(model_directory)
{
    set_color_bounds();
    initialise_object_model();
    cv::cvtColor(image_, image_hsv_, cv::COLOR_RGB2HSV);
    cv::cvtColor(image_, image_bgr_, cv::COLOR_RGB2BGR);
}

void Image::set_color_bounds()
{
    // The new colors
    // TODO: bounds not updated for new colors
    colors_["magenta"] = {"magenta", {135, 150, 70}, {175, 235, 215}};
    colors_["blue"] = {"blue", {101, 243, 81}, {112, 255, 234}};
    colors_["red"] = {"red", {0, 235, 180}, {16, 255, 255}};
    colors_["green"] = {"green", {57, 185, 60}, {70, 255, 225}};
    colors_["cyan"] = {"cyan", {75, 50, 40}, {97, 255, 180}};
    colors_["yellow"] = {"yellow", {27, 130, 160}, {37, 255, 255}};
}

void Image::initialise_object_model()
{
    object_model_[std::make_pair("yellow", "cyan")] = {0, 1};
    object_model_[std::make_pair("yellow", "magenta")] = {0, 2};
    object_model_[std::make_pair("yellow", "green")] = {1, 3};
    object_model_[std::make_pair("yellow", "blue")] = {2, 3};
    object_model_[std::make_pair("blue", "green")] = {3, 7};
    object_model_[std::make_pair("cyan", "green")] = {1, 5};
    object_model_[std::make_pair("cyan", "magenta")] = {0, 4};
    object_model_[std::make_pair("blue", "magenta")] = {2, 6};
    object_model_[std::make_pair("red", "magenta")] = {4, 6};
    object_model_[std::make_pair("red", "cyan")] = {4, 5};
    object_model_[std::make_pair("red", "green")] = {5, 7};
    object_model_[std::make_pair("red", "blue")] = {6, 7};

    for (auto &i : object_model_)
    {
        object_model_[std::make_pair(i.first.second, i.first.first)] = i.second;
    }
}

void Image::initialize_variables()
{
    for (auto &color : colors_)
    {
        // global variable initializer
        if (masks_.find(color.first) == masks_.end())
        {
            cv::Mat m(cv::Size(1, image_.rows * image_.cols),
                      CV_64FC1,
                      cv::Scalar(0));
            masks_[color.first] = m;
        }

        if (pixel_idx_.find(color.first) == pixel_idx_.end())
        {
            std::vector<int> idx;
            pixel_idx_[color.first] = idx;
        }
    }

    // for BGR + HSV input
    //    threshold_ = {{"red", -38.0},
    //                 {"green", -34.0},
    //                 {"blue", -35.0},
    //                 {"yellow", -35.0},
    //                 {"magenta", -33.0},
    //                 {"cyan", -35.0}};

    // for HSV input
    threshold_ = {{"red", -18.0},
                  {"green", -18.0},
                  {"blue", -16.0},
                  {"yellow", -20.0},
                  {"magenta", -18.0},
                  {"cyan", -16.0}};
}

void Image::run_line_detection()
{
    initialize_variables();

    auto start = std::chrono::high_resolution_clock::now();
    gmm_mask();
    auto finish = std::chrono::high_resolution_clock::now();
    //std::cout << "GMM masks took "
    //          << std::chrono::duration_cast<std::chrono::milliseconds>(finish -
    //                                                                   start)
    //                 .count()
    //          << " milliseconds\n";

    find_dominant_colors(3);

    start = std::chrono::high_resolution_clock::now();
    bool status = denoise();
    finish = std::chrono::high_resolution_clock::now();
    //std::cout << "denoise "
    //          << std::chrono::duration_cast<std::chrono::milliseconds>(finish -
    //                                                                   start)
    //                 .count()
    //          << " milliseconds\n";

    for (auto &i : dominant_colors_)
    {
        cv::Mat output;
        cv::bitwise_and(image_bgr_, image_bgr_, output, masks_[i.first]);
    }

    std::vector<std::pair<std::string, std::string>> color_pairs =
        make_valid_combinations();
    for (auto &i : color_pairs)
    {
        //std::cout << i.first << " " << i.second << std::endl;
        get_line_between_colors(i.first, i.second);
    }
}

void Image::gmm_mask()
{
    start_timer();
    cv::Mat concatenated_data;
    cv::Mat data = image_hsv_.reshape(1, image_hsv_.rows * image_hsv_.cols);
    data.convertTo(data, CV_64FC1);
    concatenated_data = data;
    finish_timer(true, "concatenation");

    start_timer();
    input_data = arma::mat(reinterpret_cast<double *>(concatenated_data.data),
                           concatenated_data.cols,
                           concatenated_data.rows);
    finish_timer(true, "cv::Mat -> arma:mat ");

    // GMM
    start_timer();
    int color_idx = 0;
    std::vector<std::thread> thread_vector;
    for (auto &color : colors_)
    {
        // getting GMM score for each color
        std::thread th(&Image::gmm_lop_p, this, color.first, color_idx);
        thread_vector.push_back(move(th));

        // used for getting argmax -> color
        idx2color_[color_idx] = color.first;
        color_idx++;
    }

    for (std::thread &th : thread_vector)
    {
        if (th.joinable()) th.join();
    }

    finish_timer(true, "masking took ");

    // ARGMAX
    start_timer();
    thread_vector.clear();
    for (int row_idx = 0; row_idx < data.rows; row_idx += 2)
    {
        auto max_idx = gmm_result_.col(row_idx).index_max();
        auto max_val = gmm_result_.col(row_idx).max();
        std::string color = idx2color_[max_idx];
        if (max_val > -18.0)
        {
            pixel_idx_[color].push_back(row_idx);
            pixel_idx_[color].push_back(row_idx + 1);
        }
    }

    finish_timer(true, "Arg Max ");

    // Background cleaning and reshaping
    start_timer();
    thread_vector.clear();
    for (auto &color : colors_)
    {
        std::thread th(&Image::create_final_mask, this, color.first);
        thread_vector.push_back(move(th));
    }
    for (std::thread &th : thread_vector)
    {
        if (th.joinable()) th.join();
    }
    finish_timer(true, "final cleaning and reshaping ");
}

void Image::create_final_mask(const std::string &color)
{
    clean_mask(color);
    for (auto &idx : pixel_idx_[color])
    {
        masks_[color].at<double>(idx, 0) = 255.0;
    }
    masks_[color] = masks_[color].reshape(1, image_.rows);
    masks_[color].convertTo(masks_[color], CV_8U);
    create_pixel_dataset(color);
}

void Image::arg_max(const int idx)
{
    for (int row_idx = idx; row_idx < idx + 1000; row_idx++)
    {
        auto max_idx = gmm_result_.col(row_idx).index_max();
        auto max_val = gmm_result_.col(row_idx).max();
        std::string color = idx2color_[max_idx];
        if (max_val > threshold_[color])
        {
            pixel_idx_[color].push_back(row_idx);
        }
    }
}

void Image::gmm_lop_p(const std::string &color, const int row_idx)
{
    arma::gmm_diag model;
    std::string saved_model_path =
        model_directory_ + "/" + color + "_diag_hsv.gmm";
    bool status = model.load(saved_model_path);
    if (status == false)
    {
        throw std::runtime_error("Failed to load GMM for " + color);
    }

    auto start = std::chrono::high_resolution_clock::now();
    arma::rowvec result = model.log_p(input_data);
    auto finish = std::chrono::high_resolution_clock::now();
    //    std::cout << color << " "
    //              <<
    //              std::chrono::duration_cast<std::chrono::milliseconds>(finish
    //              - start).count()
    //              << " milliseconds\n";

    gmm_result_.row(row_idx) = result;
}

void Image::clean_mask(const std::string &color)
{
    auto start = std::chrono::high_resolution_clock::now();
    if (color == "cyan")
    {
        cv::Mat mask;
        std::vector<int> merged(pixel_idx_[color]);
        auto lower = colors_[color].lower;
        auto upper = colors_[color].upper;
        int n = image_.rows * image_.cols;
        cv::inRange(image_hsv_,
                    cv::Scalar(lower[0], lower[1], lower[2]),
                    cv::Scalar(upper[0], upper[1], upper[2]),
                    mask);
        mask = mask.reshape(1, n);
        mask.convertTo(mask, CV_64FC1);
        std::vector<int> idx2;
        for (int i = 0; i < n; i++)
        {
            if ((float)mask.at<double>(i, 0) == 255)
            {
                idx2.push_back(i);
            }
        }
        std::vector<int> final_idx(merged.size() + idx2.size());
        std::vector<int>::iterator it =
            std::set_intersection(merged.begin(),
                                  merged.end(),
                                  idx2.begin(),
                                  idx2.end(),
                                  final_idx.begin());
        final_idx.resize(it - final_idx.begin());
        pixel_idx_[color] = final_idx;
    }

    if (color == "green")
    {
        cv::Mat mask;
        std::vector<int> merged(pixel_idx_[color]);
        auto lower = colors_[color].lower;
        auto upper = colors_[color].upper;
        int n = image_.rows * image_.cols;
        cv::inRange(
            image_hsv_, cv::Scalar(0, 0, 0), cv::Scalar(73, 255, 95), mask);
        mask = mask.reshape(1, n);
        mask.convertTo(mask, CV_64FC1);
        std::vector<int> idx2;
        for (int i = 0; i < n; i++)
        {
            if ((float)mask.at<double>(i, 0) == 0)
            {
                idx2.push_back(i);
            }
        }
        std::vector<int> final_idx(merged.size() + idx2.size());
        std::vector<int>::iterator it =
            std::set_intersection(merged.begin(),
                                  merged.end(),
                                  idx2.begin(),
                                  idx2.end(),
                                  final_idx.begin());
        final_idx.resize(it - final_idx.begin());
        pixel_idx_[color] = final_idx;
    }

    if (color == "yellow")
    {
        cv::Mat mask;
        std::vector<int> merged(pixel_idx_[color]);
        auto lower = colors_[color].lower;
        auto upper = colors_[color].upper;
        int n = image_.rows * image_.cols;
        cv::inRange(
            image_hsv_, cv::Scalar(0, 0, 0), cv::Scalar(73, 255, 95), mask);
        mask = mask.reshape(1, n);
        mask.convertTo(mask, CV_64FC1);
        std::vector<int> idx2;
        for (int i = 0; i < n; i++)
        {
            if ((float)mask.at<double>(i, 0) == 0)
            {
                idx2.push_back(i);
            }
        }
        //        std::sort(merged.begin(), merged.end());
        //        std::sort(idx2.begin(), idx2.end());
        std::vector<int> final_idx(merged.size() + idx2.size());
        std::vector<int>::iterator it =
            std::set_intersection(merged.begin(),
                                  merged.end(),
                                  idx2.begin(),
                                  idx2.end(),
                                  final_idx.begin());
        final_idx.resize(it - final_idx.begin());
        pixel_idx_[color] = final_idx;
    }

    auto finish = std::chrono::high_resolution_clock::now();
    //    std::cout << "GMM masks took "
    //              <<
    //              std::chrono::duration_cast<std::chrono::milliseconds>(finish
    //              - start).count()
    //              << " milliseconds\n";
}

void Image::create_pixel_dataset(const std::string &color)
{
    std::vector<cv::Point> poi;
    cv::findNonZero(masks_[color], poi);

    pixel_dataset_[color] = {poi};
    color_count_[color] = poi.size();
}

void Image::find_dominant_colors(const int N_dominant_colors)
{  // N dominant colors
    dominant_colors_.clear();
    std::map<std::string, int> color_count_copy(color_count_);

    int contains_blue = 0;
    int contains_cyan = 0;

    for (auto &i : color_count_copy)
    {
        if (i.second > 100)
        {
            if (i.first == "blue")
            {
                contains_blue = 1;
            }
            if (i.first == "cyan")
            {
                contains_cyan = 1;
            }

            if (contains_blue == 1 && contains_cyan == 1)
            {
                if (color_count_["blue"] > color_count_["cyan"])
                {
                    color_count_.erase("cyan");
                    contains_cyan = 0;
                }
                else
                {
                    color_count_.erase("blue");
                    contains_blue = 0;
                }
            }
            else
            {
                std::pair<std::string, int> c =
                    std::make_pair(i.first, i.second);
                dominant_colors_.insert(c);
            }
        }
        else
        {
            std::cout << "Erasing " << i.first << std::endl;
            color_count_.erase(i.first);
        }
    }

    // Removing unwanted entries from the last
    std::set<std::pair<std::string, int>>::iterator it;
    while (dominant_colors_.size() > N_dominant_colors)
    {
        auto aa = *std::prev(dominant_colors_.end());
        dominant_colors_.erase(prev(dominant_colors_.end()));
        color_count_.erase(aa.first);
    }
}

bool Image::denoise()
{
    if (dominant_colors_.size() == 0)
    {
        return true;
    }
    int erosion_size = 4;

    start_timer();
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT,
        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
        cv::Point(erosion_size, erosion_size));
    finish_timer(true, "dilation kernel ");

    start_timer();
    std::cout << "Denoising\n";
    cv::Mat merged_mask(
        cv::Size(1, image_.rows * image_.cols), CV_8U, cv::Scalar(0));
    std::vector<int> merged_idx;

    for (auto &color : dominant_colors_)
    {
        std::cout << color.first << std::endl;
        cv::Mat mask = masks_[color.first];
        mask = mask.reshape(1, image_.rows * image_.cols);
        for (int i = 0; i < mask.rows; i++)
        {
            if (mask.at<uchar>(i, 0) == 255)
            {
                merged_idx.push_back(i);
                merged_mask.at<uchar>(i, 0) = 255;
            }
        }
    }

    merged_mask = merged_mask.reshape(1, image_.rows);

    cv::Mat dilated_mask, labels_im;
    cv::dilate(merged_mask, dilated_mask, kernel, cv::Point(-1, -1), 4);
    finish_timer(true, "creating merged mask ");

    start_timer();
    int num_labels = cv::connectedComponents(dilated_mask, labels_im);
    finish_timer(true, "connected components ");

    start_timer();
    labels_im = labels_im.reshape(1, image_.rows * image_.cols);
    std::vector<int> unique_;
    for (int i = 0; i < labels_im.rows; i++)
    {
        unique_.push_back(labels_im.at<uchar>(i, 0));
    }
    std::sort(unique_.begin(), unique_.end());
    int uniqueCount =
        std::unique(unique_.begin(), unique_.end()) - unique_.begin();
    label_count_.clear();
    for (int i = 0; i < uniqueCount; i++)
    {
        label_count_.insert(
            std::make_pair(i, std::count(unique_.begin(), unique_.end(), i)));
    }
    finish_timer(true, "unique label with count ");

    start_timer();
    int element_number = 1;  // 1 because 0 is for background
    std::vector<int> idx;
    float percentage_overlap = 40.0;
    do
    {
        int max_count_label = getNthElement(element_number);
        idx.clear();
        for (int i = 0; i < labels_im.rows; i++)
        {
            if (labels_im.at<uchar>(i, 0) == max_count_label)
            {
                idx.push_back(i);
            }
        }
        std::vector<int> final_idx(merged_idx.size() + idx.size());
        std::vector<int>::iterator it =
            std::set_intersection(merged_idx.begin(),
                                  merged_idx.end(),
                                  idx.begin(),
                                  idx.end(),
                                  final_idx.begin());
        final_idx.resize(it - final_idx.begin());
        percentage_overlap =
            final_idx.size() / (merged_idx.size() + 1e-9) * 100;
        element_number++;
        std::cout << "Here " << percentage_overlap << std::endl;
    } while (percentage_overlap < 25.0);
    finish_timer(true, "finding the right label took ");

    start_timer();
    for (auto &color : dominant_colors_)
    {
        int j = 0;
        cv::Mat mask = masks_[color.first];
        mask = mask.reshape(1, image_.rows * image_.cols);
        for (int i = 0; i < mask.rows; i++)
        {
            if (mask.at<uchar>(i, 0) == 255)
            {
                auto it = std::find(idx.begin() + j, idx.end(), i);
                if (it == idx.end())
                {
                    mask.at<uchar>(i, 0) = 0;
                }
                else
                {
                    j = it - idx.begin();
                }
            }
        }
        masks_[color.first] = mask.reshape(1, image_.rows);
        create_pixel_dataset(color.first);
    }
    finish_timer(true, "getting overlap ");

    find_dominant_colors(3);
    return true;
}

int Image::getNthElement(int n)
{
    if (n >= label_count_.size())
    {
        std::cout << "Not enough elements\n";
        return -1;
    }
    std::pair<int, int> elem = *std::next(label_count_.begin(), n);
    return elem.first;
}

void Image::show()
{
    cv::Mat bgr_image;
    cv::cvtColor(image_, bgr_image, cv::COLOR_HSV2BGR);
    cv::imshow("Image", bgr_image);

    cv::Mat segmentation(
        image_.rows, image_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    std::cout << "Dominant colours: ";
    for (auto &m : dominant_colors_)
    {
        std::cout << m.first << ", ";

        auto bounds = colors_[m.first];
        // use the average of upper and lower bound to get a colour for
        // visualization
        cv::Scalar color((bounds.upper[0] + bounds.lower[0]) / 2,
                         (bounds.upper[1] + bounds.lower[1]) / 2,
                         (bounds.upper[2] + bounds.lower[2]) / 2);

        segmentation.setTo(color, masks_[m.first]);
    }
    std::cout << std::endl;
    cv::cvtColor(segmentation, bgr_image, cv::COLOR_HSV2BGR);
    cv::imshow("Segmentation", bgr_image);
    cv::waitKey(0);
}

cv::Mat Image::get_segmented_image()
{
    cv::Mat segmentation(
        image_.rows, image_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto &m : dominant_colors_)
    {
        auto bounds = colors_[m.first];
        // use the average of upper and lower bound to get a colour for
        // visualization
        cv::Scalar color((bounds.upper[0] + bounds.lower[0]) / 2,
                         (bounds.upper[1] + bounds.lower[1]) / 2,
                         (bounds.upper[2] + bounds.lower[2]) / 2);

        segmentation.setTo(color, masks_[m.first]);
    }
    cv::cvtColor(segmentation, segmentation, cv::COLOR_HSV2RGB);
    return segmentation.clone();
}

cv::Mat Image::get_segmented_image_wout_outliers()
{
    cv::Mat segmentation(
        image_.rows, image_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto &m : dominant_colors_)
    {
        auto bounds = colors_[m.first];
        // use the average of upper and lower bound to get a colour for
        // visualization
        cv::Vec3b color((bounds.upper[0] + bounds.lower[0]) / 2,
                        (bounds.upper[1] + bounds.lower[1]) / 2,
                        (bounds.upper[2] + bounds.lower[2]) / 2);
        for (auto &d : pixel_dataset_[m.first])
        {
            segmentation.at<cv::Vec3b>(d.y, d.x) = color;
        }
    }
    cv::cvtColor(segmentation, segmentation, cv::COLOR_HSV2RGB);
    return segmentation.clone();
}

cv::Mat Image::get_image()
{
    return image_.clone();
}

cv::Mat Image::get_image_lines()
{
    cv::Mat segmentation(
        image_.rows, image_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto &m : dominant_colors_)
    {
        auto bounds = colors_[m.first];
        // use the average of upper and lower bound to get a colour for
        // visualization
        cv::Vec3b color((bounds.upper[0] + bounds.lower[0]) / 2,
                        (bounds.upper[1] + bounds.lower[1]) / 2,
                        (bounds.upper[2] + bounds.lower[2]) / 2);
        for (auto &d : pixel_dataset_[m.first])
        {
            segmentation.at<cv::Vec3b>(d.y, d.x) = color;
        }
    }
    for (auto &line : lines_)
    {
        float a = line.second.first;
        float b = line.second.second;
        int xmin = 0;
        int xmax = image_.cols;

        cv::Point p1((a * xmin) + b, xmin);
        cv::Point p2((a * xmax) + b, xmax);
        cv::line(segmentation, p1, p2, cv::Scalar(20, 100, 100), 10);
    }
    cv::cvtColor(segmentation, segmentation, cv::COLOR_HSV2RGB);
    return segmentation.clone();
}

void Image::print_pixels()
{
    for (auto &i : pixel_dataset_)
    {
        std::cout << i.first << "\n";
        for (auto p : i.second)
        {
            std::cout << p.x << ", " << p.y << " ";
        }
    }
}

std::vector<std::pair<std::string, std::string>>
Image::make_valid_combinations()
{
    std::vector<std::string> color_keys;
    std::vector<std::pair<std::string, std::string>> color_pairs;
    transform(dominant_colors_.begin(),
              dominant_colors_.end(),
              back_inserter(color_keys),
              [](std::pair<std::string, int> elem) { return elem.first; });
    for (auto it = color_keys.begin(); it != color_keys.end(); it++)
    {
        for (auto it2 = it + 1; it2 != color_keys.end(); it2++)
        {
            auto p = std::make_pair(*it, *it2);
            auto idx = object_model_.find(p);
            if (idx != object_model_.end()) color_pairs.push_back(p);
        }
    }
    return color_pairs;
}

void Image::get_line_between_colors(const std::string &c1,
                                    const std::string &c2)
{
    std::vector<cv::Point2f> classifier_input_data;
    std::vector<int> classifier_output_data;

    if (pixel_dataset_[c1].size() > 65 && pixel_dataset_[c2].size() > 65)
    {
        for (auto &i : pixel_dataset_[c1])
        {
            classifier_input_data.push_back(cv::Point2f(i.y, i.x));
            classifier_output_data.push_back(-1);
        }
        for (auto &i : pixel_dataset_[c2])
        {
            classifier_input_data.push_back(cv::Point2f(i.y, i.x));
            classifier_output_data.push_back(1);
        }

        cv::Mat ip = cv::Mat(classifier_input_data.size(),
                             2,
                             CV_32FC1,
                             classifier_input_data.data());
        //        cv::Mat op = cv::Mat(classifier_output_data.size(), 1, CV_32F,
        //        classifier_output_data.data());

        std::string classifier = "svm";  //"sklearn_logistic";
        float a, b;
        float accuracy_;
        // Train the SVM

        if (classifier == "svm")
        {
            cv::Mat op = cv::Mat(classifier_output_data.size(),
                                 1,
                                 CV_32S,
                                 classifier_output_data.data());
            cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
            svm->setType(cv::ml::SVM::NU_SVC);
            svm->setKernel(cv::ml::SVM::LINEAR);
            svm->setC(5);
            svm->setP(0.9);
            svm->setNu(0.05);
            svm->setGamma(0);
            svm->setTermCriteria(
                cv::TermCriteria(cv::TermCriteria::EPS, 1000, 1e-6));

            try
            {
                svm->train(ip, cv::ml::ROW_SAMPLE, op);
            }
            catch (int e)
            {
                std::cerr << "Exception caught : " << e << std::endl;
            }

            if (output_percentage_classification == 1)
            {
                cv::Mat responses;
                svm->predict(ip, responses);
                responses.convertTo(responses, CV_32S);
                std::cout << "accuracy: "
                          << calculateAccuracyPercent(op, responses) << "%"
                          << std::endl;

                accuracy_ = calculateAccuracyPercent(op, responses);
            }
            cv::Mat alpha, svidx;
            cv::Mat sv = svm->getSupportVectors();
            float rho = svm->getDecisionFunction(0, alpha, svidx);
            a = -sv.at<float>(0, 0) / sv.at<float>(0, 1);
            b = rho / sv.at<float>(0, 1);

            std::cout << "\n*************\n";
            std::cout << "Rho " << rho << " first " << -sv.at<float>(0, 0)
                      << " divide_by " << sv.at<float>(0, 1) << std::endl;
            std::cout << a << " " << b << std::endl;
        }
        else if (classifier == "logistic")
        {
            cv::Mat op = cv::Mat(classifier_output_data.size(),
                                 1,
                                 CV_32F,
                                 classifier_output_data.data());
            cv::Ptr<cv::ml::LogisticRegression> lr =
                cv::ml::LogisticRegression::create();
            lr->setLearningRate(0.001);
            lr->setTrainMethod(cv::ml::LogisticRegression::MINI_BATCH);
            lr->setMiniBatchSize(2);
            lr->setIterations(100);
            lr->train(ip, cv::ml::ROW_SAMPLE, op);

            auto thetas = lr->get_learnt_thetas();
            //    std::cout << "\nPrinting Thetas for " << c1 << " and " << c2
            //    <<std::endl; std::cout << thetas << " " <<
            //    thetas.at<float>(0,0) << " "
            //    << thetas.at<float>(0,1) << " " << thetas.at<float>(0,2) <<
            //    std::endl;

            a = -thetas.at<float>(0, 0) / thetas.at<float>(0, 1);
            b = thetas.at<float>(0, 2) / thetas.at<float>(0, 1);
        }

        if (accuracy_ > 90.0)
        {
            lines_[std::make_pair(c1, c2)] = {a, b};
        }
    }
}

cv::Mat Image::get_mask(const std::string &color_name)
{
    return masks_[color_name];
}

void Image::start_timer()
{
    start_ = std::chrono::high_resolution_clock::now();
}

void Image::finish_timer(bool verbose = false, const std::string &message = "")
{
    finish_ = std::chrono::high_resolution_clock::now();

    if (verbose)
    {
        print_time_taken(message);
    }
}

void Image::print_time_taken(const std::string &message = "")
{
    std::cout << message << " "
              << std::chrono::duration_cast<std::chrono::milliseconds>(finish_ -
                                                                       start_)
                     .count()
              << " milliseconds\n";
}

}  // namespace trifinger_object_tracking
