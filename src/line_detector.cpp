#include <math.h>
#include <trifinger_object_tracking/xgboost_classifier.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <trifinger_object_tracking/gmm_params.hpp>
#include <trifinger_object_tracking/line_detector.hpp>
#include <trifinger_object_tracking/scoped_timer.hpp>
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

LineDetector::LineDetector(const CubeModel &cube_model,
                           const std::string &model_directory)
    : cube_model_(cube_model)
{
    set_color_bounds();
    load_segmentation_models(model_directory);
}

void LineDetector::set_color_bounds()
{
    // The new colors
    // TODO: bounds not updated for new colors
    // TODO: are these bounds still needed?
    color_bounds_[FaceColor::MAGENTA] = {{135, 150, 70}, {175, 235, 215}};
    color_bounds_[FaceColor::BLUE] = {{101, 243, 81}, {112, 255, 234}};
    color_bounds_[FaceColor::RED] = {{0, 235, 180}, {16, 255, 255}};
    color_bounds_[FaceColor::GREEN] = {{57, 185, 60}, {70, 255, 225}};
    color_bounds_[FaceColor::CYAN] = {{75, 50, 40}, {97, 255, 180}};
    color_bounds_[FaceColor::YELLOW] = {{27, 130, 160}, {37, 255, 255}};

    gmm_thresholds_[FaceColor::YELLOW] = -24;
    gmm_thresholds_[FaceColor::RED] = -19;
    gmm_thresholds_[FaceColor::GREEN] = -19;
    gmm_thresholds_[FaceColor::BLUE] = -12;
    gmm_thresholds_[FaceColor::MAGENTA] = -19;
    gmm_thresholds_[FaceColor::CYAN] = -23;
}

void LineDetector::load_segmentation_models(const std::string &model_directory)
{
    std::string model_file = model_directory + "/gmm_weights_from_python.yml";
    segmentation_models_ =
        trifinger_object_tracking::load_gmm_models_from_file(model_file);
}

std::map<ColorPair, Line> LineDetector::detect_lines(const cv::Mat &image_bgr)
{
    ScopedTimer timer("LineDetector/detect_lines");

    // TODO better solution than class members
    image_bgr_ = image_bgr;
    cv::cvtColor(image_bgr_, image_hsv_, cv::COLOR_BGR2HSV);

    lines_.clear();

    // gmm_mask();
    xgboost_mask();
    find_dominant_colors(3);
    deflate_masks_of_dominant_colors();

    for (auto [color1, color2] : make_valid_combinations())
    {
        get_line_between_colors(color1, color2);
    }

    return lines_;
}

void LineDetector::xgboost_mask()
{
    ScopedTimer timer("LineDetector/xgboost_mask");

    constexpr std::array<FaceColor, FaceColor::N_COLORS> map_label_to_color = {
        FaceColor::BLUE,
        FaceColor::CYAN,
        FaceColor::GREEN,
        FaceColor::MAGENTA,
        FaceColor::RED,
        FaceColor::YELLOW,
    };

    // Background cleaning and reshaping
    constexpr unsigned OPEN_RADIUS = 2;
    static const cv::Mat open_kernel = cv::getStructuringElement(
        cv::MORPH_ELLIPSE, cv::Size(2 * OPEN_RADIUS + 1, 2 * OPEN_RADIUS + 1));

    // initialize masks
    for (FaceColor color : cube_model_.get_colors())
    {
        masks_[color] =
            cv::Mat(image_bgr_.rows, image_bgr_.cols, CV_8UC1, cv::Scalar(0));
    }

    const size_t n_pixels = image_bgr_.total();

    for (int r = 0; r < image_bgr_.rows; r++)
    {
        for (int c = 0; c < image_bgr_.cols; c++)
        {
            size_t i = r * image_bgr_.rows + c;

            std::array<float, XGB_NUM_FEATURES> features;

            cv::Vec3b bgr = image_bgr_.at<cv::Vec3b>(r, c);
            cv::Vec3b hsv = image_hsv_.at<cv::Vec3b>(r, c);

            features[0] = static_cast<float>(bgr[0]);
            features[1] = static_cast<float>(bgr[1]);
            features[2] = static_cast<float>(bgr[2]);
            features[3] = static_cast<float>(hsv[0]);
            features[4] = static_cast<float>(hsv[1]);
            features[5] = static_cast<float>(hsv[2]);

            std::array<float, XGB_NUM_CLASSES> probabilities = xgb_classify(features);

            auto max_elem =
                std::max_element(probabilities.begin(), probabilities.end());
            int label = max_elem - probabilities.begin();

            if (label > 0)
            {
                FaceColor color = map_label_to_color[label - 1];
                masks_[color].at<uint8_t>(r, c) = 255;
            }
        }
    }

    // post-process masks
    for (FaceColor color : cube_model_.get_colors())
    {
        // "open" image to get rid of single-pixel noise
        cv::morphologyEx(
            masks_[color], masks_[color], cv::MORPH_OPEN, open_kernel);

        // count number of pixels of each color
        color_count_[color] = cv::countNonZero(masks_[color]);
    }
}

void LineDetector::gmm_mask()
{
    ScopedTimer timer("LineDetector/gmm_mask");

    const size_t n_pixels = image_bgr_.total();

    arma::mat gmm_result =
        arma::mat(FaceColor::N_COLORS, n_pixels, arma::fill::zeros);

    std::map<int, FaceColor> idx2color;
    std::array<std::vector<int>, FaceColor::N_COLORS> pixel_idx;

    // initialize masks
    for (FaceColor color : cube_model_.get_colors())
    {
        masks_[color] = cv::Mat(cv::Size(1, n_pixels), CV_64FC1, cv::Scalar(0));
    }

    // convert cv::Mat to arma::mat
    cv::Mat concatenated_data;
    cv::Mat data_hsv = image_hsv_.reshape(1, n_pixels);
    cv::Mat data_bgr = image_bgr_.reshape(1, n_pixels);
    data_hsv.convertTo(data_hsv, CV_64FC1);
    data_bgr.convertTo(data_bgr, CV_64FC1);
    cv::hconcat(data_bgr, data_hsv, concatenated_data);  // bgr+hsv
    arma::mat input_data =
        arma::mat(reinterpret_cast<double *>(concatenated_data.data),
                  concatenated_data.cols,
                  concatenated_data.rows);

    // GMM
    int color_idx = 0;
    std::vector<std::thread> thread_vector;
    for (FaceColor color : cube_model_.get_colors())
    {
        // getting GMM score for each color
        std::thread th(
            [this, &input_data, &gmm_result](FaceColor color,
                                             const int row_idx) {
                gmm_result.row(row_idx) =
                    segmentation_models_[color].log_p(input_data);
            },
            color,
            color_idx);
        thread_vector.push_back(move(th));

        // used for getting argmax -> color
        idx2color[color_idx] = color;
        color_idx++;
    }

    for (std::thread &th : thread_vector)
    {
        if (th.joinable()) th.join();
    }

    // ARGMAX
    thread_vector.clear();
    for (int row_idx = 0; row_idx < data_hsv.rows; row_idx += 2)
    {
        auto max_idx = gmm_result.col(row_idx).index_max();
        auto max_val = gmm_result.col(row_idx).max();
        FaceColor color = idx2color[max_idx];
        if (max_val > gmm_thresholds_[color])
        {
            pixel_idx[color].push_back(row_idx);
            pixel_idx[color].push_back(row_idx + 1);
        }
    }

    // Background cleaning and reshaping
    constexpr unsigned OPEN_RADIUS = 2;
    static const cv::Mat open_kernel = cv::getStructuringElement(
        cv::MORPH_ELLIPSE, cv::Size(2 * OPEN_RADIUS + 1, 2 * OPEN_RADIUS + 1));

    thread_vector.clear();
    for (FaceColor color : cube_model_.get_colors())
    {
        std::thread th(
            [this, &pixel_idx, &open_kernel](FaceColor color) {
                clean_mask(color, pixel_idx);
                for (auto &idx : pixel_idx[color])
                {
                    masks_[color].at<double>(idx, 0) = 255.0;
                }
                masks_[color] = masks_[color].reshape(1, image_bgr_.rows);
                masks_[color].convertTo(masks_[color], CV_8U);

                // "open" image to get rid of single-pixel noise
                cv::morphologyEx(
                    masks_[color], masks_[color], cv::MORPH_OPEN, open_kernel);

                color_count_[color] = cv::countNonZero(masks_[color]);
            },
            color);
        thread_vector.push_back(move(th));
    }
    for (std::thread &th : thread_vector)
    {
        if (th.joinable()) th.join();
    }
}

// TODO what exactly is this doing?
void LineDetector::clean_mask(
    FaceColor color,
    std::array<std::vector<int>, FaceColor::N_COLORS> &pixel_idx)
{
    switch (color)
    {
        case FaceColor::CYAN:
        {
            cv::Mat mask;
            std::vector<int> merged(pixel_idx[color]);
            auto lower = color_bounds_[color].lower;
            auto upper = color_bounds_[color].upper;
            int n = image_hsv_.rows * image_hsv_.cols;
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
            pixel_idx[color] = final_idx;
        }
        break;

        case FaceColor::GREEN:
        {
            cv::Mat mask;
            std::vector<int> merged(pixel_idx[color]);
            int n = image_hsv_.rows * image_hsv_.cols;
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
            pixel_idx[color] = final_idx;
        }
        break;

        case FaceColor::YELLOW:
        {
            cv::Mat mask;
            std::vector<int> merged(pixel_idx[color]);
            int n = image_hsv_.rows * image_hsv_.cols;
            // FIXME these are the same bounds as above for GREEN.  Does this
            // make sense?
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
            pixel_idx[color] = final_idx;
        }
        break;

        default:
            // do nothing
            break;
    }
}

void LineDetector::find_dominant_colors(const unsigned int N_dominant_colors)
{
    ScopedTimer timer("LineDetector/find_dominant_colors");

    struct cmp
    {  // Declaring a set that will store the std::pairs using the comparator
       // logic
        bool operator()(std::pair<FaceColor, int> elem1,
                        std::pair<FaceColor, int> elem2)
        {
            return elem1.second > elem2.second;
        }
    };

    std::set<std::pair<FaceColor, int>, cmp> dominant_color_count_set;

    // N dominant colors
    std::map<FaceColor, int> color_count_copy(color_count_);

    int contains_blue = 0;
    int contains_cyan = 0;

    for (auto &i : color_count_copy)
    {
        if (i.second > 100)
        {
            if (i.first == FaceColor::BLUE)
            {
                contains_blue = 1;
            }
            if (i.first == FaceColor::CYAN)
            {
                contains_cyan = 1;
            }

            if (contains_blue == 1 && contains_cyan == 1)
            {
                if (color_count_[FaceColor::BLUE] >
                    color_count_[FaceColor::CYAN])
                {
                    color_count_.erase(FaceColor::CYAN);
                    contains_cyan = 0;
                }
                else
                {
                    color_count_.erase(FaceColor::BLUE);
                    contains_blue = 0;
                }
            }
            else
            {
                auto c = std::make_pair(i.first, i.second);
                dominant_color_count_set.insert(c);
            }
        }
        else
        {
            // std::cout << "Erasing " << i.first << std::endl;
            color_count_.erase(i.first);
        }
    }

    // Removing unwanted entries from the last
    while (dominant_color_count_set.size() > N_dominant_colors)
    {
        auto aa = *std::prev(dominant_color_count_set.end());
        dominant_color_count_set.erase(prev(dominant_color_count_set.end()));
        color_count_.erase(aa.first);
    }

    // copy the final dominant colours to dominant_colors_
    dominant_colors_.clear();
    transform(dominant_color_count_set.begin(),
              dominant_color_count_set.end(),
              back_inserter(dominant_colors_),
              [](std::pair<FaceColor, int> elem) { return elem.first; });
}

void LineDetector::deflate_masks_of_dominant_colors()
{
    ScopedTimer timer("LineDetector/deflate_masks_of_dominant_colors");

    // TODO: what is a good value?
    constexpr unsigned DEFLATION_RADIUS = 7;

    static const cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_ELLIPSE,
        cv::Size(2 * DEFLATION_RADIUS + 1, 2 * DEFLATION_RADIUS + 1));

    for (FaceColor color : dominant_colors_)
    {
        cv::dilate(masks_[color], deflated_masks_[color], kernel);
    }
}

void LineDetector::show()
{
    cv::imshow("Image", image_bgr_);

    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    // std::cout << "Dominant colours: ";
    for (FaceColor color : dominant_colors_)
    {
        // std::cout << color << ", ";

        auto rgb = cube_model_.get_hsv(color);
        cv::Scalar color_hsv(rgb[0], rgb[1], rgb[2]);

        segmentation.setTo(color_hsv, masks_[color]);
    }
    // std::cout << std::endl;
    cv::Mat bgr_image;
    cv::cvtColor(segmentation, bgr_image, cv::COLOR_HSV2BGR);
    cv::imshow("Segmentation", bgr_image);
    cv::waitKey(0);
}

cv::Mat LineDetector::get_segmented_image() const
{
    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (FaceColor color : dominant_colors_)
    {
        auto rgb = cube_model_.get_rgb(color);
        // image is BGR, so swap R and B
        cv::Scalar color_bgr(rgb[2], rgb[1], rgb[0]);
        segmentation.setTo(color_bgr, masks_[color]);
    }
    return segmentation.clone();
}

cv::Mat LineDetector::get_front_line_image() const
{
    cv::Mat image(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto [c1, c2] : make_valid_combinations())
    {
        auto [pixels_c1, pixels_c2] = get_front_line_pixels(c1, c2);

        auto rgb1 = cube_model_.get_rgb(c1);
        auto rgb2 = cube_model_.get_rgb(c2);
        // image is BGR, so swap R and B
        cv::Vec3b color_bgr1(rgb1[2], rgb1[1], rgb1[0]);
        cv::Vec3b color_bgr2(rgb2[2], rgb2[1], rgb2[0]);

        for (auto &p : pixels_c1)
        {
            image.at<cv::Vec3b>(p.y, p.x) = color_bgr1;
        }
        for (auto &p : pixels_c2)
        {
            image.at<cv::Vec3b>(p.y, p.x) = color_bgr2;
        }
    }

    return image;
}

cv::Mat LineDetector::get_image() const
{
    return image_bgr_.clone();
}

cv::Mat LineDetector::get_image_lines() const
{
    cv::Mat segmentation = get_front_line_image();
    for (auto &line : lines_)
    {
        float a = line.second.a;
        float b = line.second.b;
        int xmin = 0;
        int xmax = image_bgr_.cols;

        cv::Point p1((a * xmin) + b, xmin);
        cv::Point p2((a * xmax) + b, xmax);
        cv::line(segmentation, p1, p2, cv::Scalar(20, 100, 100), 10);
    }
    return segmentation.clone();
}

std::vector<std::pair<FaceColor, FaceColor>>
LineDetector::make_valid_combinations() const
{
    //ScopedTimer timer("LineDetector/make_valid_combinations");

    std::vector<std::pair<FaceColor, FaceColor>> color_pairs;
    for (auto it = dominant_colors_.begin(); it != dominant_colors_.end(); it++)
    {
        for (auto it2 = it + 1; it2 != dominant_colors_.end(); it2++)
        {
            auto p = std::make_pair(*it, *it2);
            auto idx = cube_model_.object_model_.find(p);
            if (idx != cube_model_.object_model_.end())
            {
                color_pairs.push_back(p);
            }
        }
    }
    return color_pairs;
}

std::array<std::vector<cv::Point>, 2> LineDetector::get_front_line_pixels(
    FaceColor color1, FaceColor color2) const
{
    //ScopedTimer timer("LineDetector/get_front_line_pixels");

    std::array<std::vector<cv::Point>, 2> front_line_pixels;
    cv::Mat front_line;

    cv::bitwise_and(masks_[color1], deflated_masks_[color2], front_line);
    cv::findNonZero(front_line, front_line_pixels[0]);

    cv::bitwise_and(masks_[color2], deflated_masks_[color1], front_line);
    cv::findNonZero(front_line, front_line_pixels[1]);

    return front_line_pixels;
}

void LineDetector::get_line_between_colors(FaceColor c1, FaceColor c2)
{
    ScopedTimer timer("LineDetector/get_line_between_colors");

    constexpr float LINE_ACCURACY_THRESHOLD = 90.0;

    auto [pixels_c1, pixels_c2] = get_front_line_pixels(c1, c2);

    // TODO should this be changed after the filtering done above?
    constexpr size_t MIN_PIXELS_PER_COLOR = 55;

    // TODO this ratio seems really low
    constexpr float MIN_COLOR_RATIO = 0.01;

    std::vector<cv::Point2f> classifier_input_data;
    std::vector<int> classifier_output_data;

    const float color_ratio_c1 =
        pixels_c1.size() / float(pixels_c2.size() + pixels_c1.size());
    const float color_ratio_c2 =
        pixels_c2.size() / float(pixels_c2.size() + pixels_c1.size());

    std::cout << "\n*************\n";

    if (pixels_c1.size() > MIN_PIXELS_PER_COLOR &&
        pixels_c2.size() > MIN_PIXELS_PER_COLOR &&
        color_ratio_c1 > MIN_COLOR_RATIO && color_ratio_c2 > MIN_COLOR_RATIO)
    {
        std::cout << "Find line between " << c1 << " and " << c2 << std::endl;

        for (auto &pixel : pixels_c1)
        {
            classifier_input_data.push_back(cv::Point2f(pixel.y, pixel.x));
            classifier_output_data.push_back(-1);
        }
        for (auto &pixel : pixels_c2)
        {
            classifier_input_data.push_back(cv::Point2f(pixel.y, pixel.x));
            classifier_output_data.push_back(1);
        }

        // TODO could the input data be written directly to this mat instead of
        // creating a std::vector first?
        cv::Mat ip = cv::Mat(classifier_input_data.size(),
                             2,
                             CV_32FC1,
                             classifier_input_data.data());

        std::string classifier = "svm";  //"sklearn_logistic";
        float a, b;
        float accuracy;

        // Train the SVM
        if (classifier == "svm")
        {
            // TODO could the output data be written directly to this mat
            // instead of creating a std::vector first?
            cv::Mat op = cv::Mat(classifier_output_data.size(),
                                 1,
                                 CV_32S,
                                 classifier_output_data.data());

            // // just some printing ----------------------------------------
            // if ((c1 == FaceColor::MAGENTA && c2 == FaceColor::RED) ||
            //     (c2 == FaceColor::MAGENTA && c1 == FaceColor::RED))
            // {
            //     // std::cout << "input col 0: " << std::endl;
            //     // std::cout << ip.col(0).t() << std::endl;
            //     double min_row, max_row;
            //     cv::minMaxLoc(ip.col(0), &min_row, &max_row);
            //     std::cout << "min_row: " << min_row << "   max_row: " <<
            //     max_row
            //               << std::endl;

            //     double min_col, max_col;
            //     cv::minMaxLoc(ip.col(1), &min_col, &max_col);
            //     std::cout << "min_col: " << min_col << "   max_col: " <<
            //     max_col
            //               << std::endl;

            //     // std::cout << "input: " << std::endl;
            //     // std::cout << ip.t() << std::endl;
            //     // std::cout << "output: " << std::endl;
            //     // std::cout << op.t() << std::endl;
            // }

            // cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
            // svm->setType(cv::ml::SVM::NU_SVC);
            // svm->setKernel(cv::ml::SVM::LINEAR);
            // svm->setNu(0.05);
            // svm->setTermCriteria(
            //     cv::TermCriteria(cv::TermCriteria::EPS, 1000, 1e-6));

            // Ptr<SVM> svm = SVM::create();
            // svm->setType(SVM::C_SVC);
            // svm->setC(0.1);
            // svm->setKernel(SVM::LINEAR);
            // svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER,
            // (int)1e7, 1e-6));

            cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
            {
                ScopedTimer timer("SVM");
                svm->setType(cv::ml::SVM::C_SVC);
                svm->setC(0.1);
                svm->setKernel(cv::ml::SVM::LINEAR);
                svm->setTermCriteria(
                    cv::TermCriteria(cv::TermCriteria::EPS, (int)1e6, 1e-5));

                try
                {
                    svm->train(ip, cv::ml::ROW_SAMPLE, op);
                }
                catch (int e)
                {
                    std::cerr << "Exception caught : " << e << std::endl;
                }
            }

            if (output_percentage_classification == 1)
            {
                cv::Mat responses;
                svm->predict(ip, responses);
                responses.convertTo(responses, CV_32S);
                accuracy = calculateAccuracyPercent(op, responses);
                std::cout << "accuracy: " << accuracy << "%" << std::endl;
            }
            cv::Mat alpha, svidx;
            cv::Mat sv = svm->getSupportVectors();
            float rho = svm->getDecisionFunction(0, alpha, svidx);
            a = -sv.at<float>(0, 0) / sv.at<float>(0, 1);
            b = rho / sv.at<float>(0, 1);

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
            std::cout << "\nPrinting Thetas for " << c1 << " and " << c2
                      << std::endl;
            std::cout << thetas << " " << thetas.at<float>(0, 0) << " "
                      << thetas.at<float>(0, 1) << " " << thetas.at<float>(0, 2)
                      << std::endl;

            a = -thetas.at<float>(0, 0) / thetas.at<float>(0, 1);
            b = thetas.at<float>(0, 2) / thetas.at<float>(0, 1);
        }

        if (accuracy > LINE_ACCURACY_THRESHOLD)
        {
            lines_[std::make_pair(c1, c2)] = Line(a, b);
        }
    }
    else
    {
        std::cout << "Not enough pixels to get line between " << c1 << " and "
                  << c2 << std::endl;
    }
}

cv::Mat LineDetector::get_mask(FaceColor color)
{
    return masks_[color];
}

void LineDetector::start_timer()
{
    start_ = std::chrono::high_resolution_clock::now();
}

void LineDetector::finish_timer(bool verbose = false,
                                const std::string &message = "")
{
    finish_ = std::chrono::high_resolution_clock::now();

    if (verbose)
    {
        print_time_taken(message);
    }
}

void LineDetector::print_time_taken(const std::string &message = "")
{
    std::cout << message << " "
              << std::chrono::duration_cast<std::chrono::milliseconds>(finish_ -
                                                                       start_)
                     .count()
              << " milliseconds\n";
}
}  // namespace trifinger_object_tracking
