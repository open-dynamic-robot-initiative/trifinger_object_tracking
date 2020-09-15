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

Image::Image(cv::Mat img_bgr, const std::string &model_directory)
    : image_bgr_(img_bgr)
{
    set_color_bounds();
    load_segmentation_models(model_directory);

    cv::cvtColor(image_bgr_, image_hsv_, cv::COLOR_BGR2HSV);
}

void Image::set_color_bounds()
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
}

void Image::load_segmentation_models(const std::string &model_directory)
{
    for (FaceColor color : cube_model_.get_colors())
    {
        std::string saved_model_path = model_directory + "/" +
                                       cube_model_.get_color_name(color) +
                                       "_diag_hsv.gmm";

        bool status = segmentation_models_[color].load(saved_model_path);
        if (status == false)
        {
            throw std::runtime_error("Failed to load GMM from " +
                                     saved_model_path);
        }
    }
}

void Image::initialize_variables()
{
    for (FaceColor color : cube_model_.get_colors())
    {
        masks_[color] = cv::Mat(cv::Size(1, image_bgr_.rows * image_bgr_.cols),
                                CV_64FC1,
                                cv::Scalar(0));

        // TODO is this explicit clear needed?
        pixel_idx_[color].clear();
    }
}

void Image::run_line_detection()
{
    initialize_variables();

    gmm_mask();

    find_dominant_colors(3);

    denoise();

    for (auto &i : dominant_colors_)
    {
        cv::Mat output;
        cv::bitwise_and(image_bgr_, image_bgr_, output, masks_[i.first]);
    }

    std::vector<std::pair<FaceColor, FaceColor>> color_pairs =
        make_valid_combinations();
    for (auto &i : color_pairs)
    {
        get_line_between_colors(i.first, i.second);
    }
}

void Image::gmm_mask()
{
    // FIXME magic numbers
    arma::mat gmm_result =
        arma::mat(FaceColor::N_COLORS, 388800, arma::fill::zeros);

    std::map<int, FaceColor> idx2color;

    // convert cv::Mat to arma::mat
    cv::Mat concatenated_data;
    cv::Mat data = image_hsv_.reshape(1, image_hsv_.rows * image_hsv_.cols);
    data.convertTo(data, CV_64FC1);
    concatenated_data = data;
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
    for (int row_idx = 0; row_idx < data.rows; row_idx += 2)
    {
        auto max_idx = gmm_result.col(row_idx).index_max();
        auto max_val = gmm_result.col(row_idx).max();
        FaceColor color = idx2color[max_idx];
        if (max_val > -18.0)
        {
            pixel_idx_[color].push_back(row_idx);
            pixel_idx_[color].push_back(row_idx + 1);
        }
    }

    // Background cleaning and reshaping
    thread_vector.clear();
    for (FaceColor color : cube_model_.get_colors())
    {
        std::thread th(
            [this](FaceColor color) {
                clean_mask(color);
                for (auto &idx : pixel_idx_[color])
                {
                    masks_[color].at<double>(idx, 0) = 255.0;
                }
                masks_[color] = masks_[color].reshape(1, image_bgr_.rows);
                masks_[color].convertTo(masks_[color], CV_8U);
                create_pixel_dataset(color);
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
void Image::clean_mask(FaceColor color)
{
    auto start = std::chrono::high_resolution_clock::now();
    switch (color)
    {
        case FaceColor::CYAN:
        {
            cv::Mat mask;
            std::vector<int> merged(pixel_idx_[color]);
            auto lower = color_bounds_[color].lower;
            auto upper = color_bounds_[color].upper;
            int n = image_bgr_.rows * image_bgr_.cols;
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
        break;

        case FaceColor::GREEN:
        {
            cv::Mat mask;
            std::vector<int> merged(pixel_idx_[color]);
            auto lower = color_bounds_[color].lower;
            auto upper = color_bounds_[color].upper;
            int n = image_bgr_.rows * image_bgr_.cols;
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
        break;

        case FaceColor::YELLOW:
        {
            cv::Mat mask;
            std::vector<int> merged(pixel_idx_[color]);
            auto lower = color_bounds_[color].lower;
            auto upper = color_bounds_[color].upper;
            int n = image_bgr_.rows * image_bgr_.cols;
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
        break;

        default:
            // do nothing
            break;
    }

    auto finish = std::chrono::high_resolution_clock::now();
    //    std::cout << "GMM masks took "
    //              <<
    //              std::chrono::duration_cast<std::chrono::milliseconds>(finish
    //              - start).count()
    //              << " milliseconds\n";
}

void Image::create_pixel_dataset(FaceColor color)
{
    std::vector<cv::Point> poi;
    cv::findNonZero(masks_[color], poi);

    pixel_dataset_[color] = {poi};
    color_count_[color] = poi.size();
}

void Image::find_dominant_colors(const int N_dominant_colors)
{  // N dominant colors
    dominant_colors_.clear();
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

    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT,
        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
        cv::Point(erosion_size, erosion_size));

    std::cout << "Denoising\n";
    cv::Mat merged_mask(
        cv::Size(1, image_bgr_.rows * image_bgr_.cols), CV_8U, cv::Scalar(0));
    std::vector<int> merged_idx;

    for (auto &color : dominant_colors_)
    {
        std::cout << color.first << std::endl;
        cv::Mat mask = masks_[color.first];
        mask = mask.reshape(1, image_bgr_.rows * image_bgr_.cols);
        for (int i = 0; i < mask.rows; i++)
        {
            if (mask.at<uchar>(i, 0) == 255)
            {
                merged_idx.push_back(i);
                merged_mask.at<uchar>(i, 0) = 255;
            }
        }
    }

    merged_mask = merged_mask.reshape(1, image_bgr_.rows);

    cv::Mat dilated_mask, labels_im;
    cv::dilate(merged_mask, dilated_mask, kernel, cv::Point(-1, -1), 4);

    int num_labels = cv::connectedComponents(dilated_mask, labels_im);

    labels_im = labels_im.reshape(1, image_bgr_.rows * image_bgr_.cols);
    std::vector<int> unique_;
    for (int i = 0; i < labels_im.rows; i++)
    {
        unique_.push_back(labels_im.at<uchar>(i, 0));
    }
    std::sort(unique_.begin(), unique_.end());
    int uniqueCount =
        std::unique(unique_.begin(), unique_.end()) - unique_.begin();

    struct compare_label_count
    {  // Declaring a set that will store the std::pairs using the comparator
       // logic
        bool operator()(std::pair<int, int> elem1, std::pair<int, int> elem2)
        {
            return elem1.second > elem2.second;
        }
    };

    std::set<std::pair<int, int>, compare_label_count> label_count;
    for (int i = 0; i < uniqueCount; i++)
    {
        label_count.insert(
            std::make_pair(i, std::count(unique_.begin(), unique_.end(), i)));
    }

    int element_number = 1;  // 1 because 0 is for background
    std::vector<int> idx;
    float percentage_overlap = 40.0;
    do
    {
        // get n-th element from label_count
        // TODO: accessing a set with an index feels wrong.  use different data
        // structure?
        int max_count_label;
        if (element_number >= label_count.size())
        {
            std::cout << "Not enough elements\n";
            max_count_label = -1;
        }
        std::pair<int, int> elem =
            *std::next(label_count.begin(), element_number);
        max_count_label = elem.first;

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

    for (auto &color : dominant_colors_)
    {
        int j = 0;
        cv::Mat mask = masks_[color.first];
        mask = mask.reshape(1, image_bgr_.rows * image_bgr_.cols);
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
        masks_[color.first] = mask.reshape(1, image_bgr_.rows);
        create_pixel_dataset(color.first);
    }

    find_dominant_colors(3);
    return true;
}

void Image::show()
{
    cv::imshow("Image", image_bgr_);

    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    std::cout << "Dominant colours: ";
    for (auto &m : dominant_colors_)
    {
        std::cout << m.first << ", ";

        auto rgb = cube_model_.get_hsv(m.first);
        cv::Scalar color(rgb[0], rgb[1], rgb[2]);

        segmentation.setTo(color, masks_[m.first]);
    }
    std::cout << std::endl;
    cv::Mat bgr_image;
    cv::cvtColor(segmentation, bgr_image, cv::COLOR_HSV2BGR);
    cv::imshow("Segmentation", bgr_image);
    cv::waitKey(0);
}

cv::Mat Image::get_segmented_image()
{
    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto &m : dominant_colors_)
    {
        auto rgb = cube_model_.get_rgb(m.first);
        // image is BGR, so swap R and B
        cv::Scalar color(rgb[2], rgb[1], rgb[0]);
        segmentation.setTo(color, masks_[m.first]);
    }
    return segmentation.clone();
}

cv::Mat Image::get_segmented_image_wout_outliers()
{
    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto &m : dominant_colors_)
    {
        auto rgb = cube_model_.get_rgb(m.first);
        // image is BGR, so swap R and B
        cv::Vec3b color(rgb[2], rgb[1], rgb[0]);

        for (auto &d : pixel_dataset_[m.first])
        {
            segmentation.at<cv::Vec3b>(d.y, d.x) = color;
        }
    }
    return segmentation.clone();
}

cv::Mat Image::get_image()
{
    return image_bgr_.clone();
}

cv::Mat Image::get_image_lines()
{
    cv::Mat segmentation(
        image_bgr_.rows, image_bgr_.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (auto &m : dominant_colors_)
    {
        auto rgb = cube_model_.get_rgb(m.first);
        // image is BGR, so swap R and B
        cv::Vec3b color(rgb[2], rgb[1], rgb[0]);
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
        int xmax = image_bgr_.cols;

        cv::Point p1((a * xmin) + b, xmin);
        cv::Point p2((a * xmax) + b, xmax);
        cv::line(segmentation, p1, p2, cv::Scalar(20, 100, 100), 10);
    }
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

std::vector<std::pair<FaceColor, FaceColor>> Image::make_valid_combinations()
{
    std::vector<FaceColor> color_keys;
    std::vector<std::pair<FaceColor, FaceColor>> color_pairs;
    transform(dominant_colors_.begin(),
              dominant_colors_.end(),
              back_inserter(color_keys),
              [](std::pair<FaceColor, int> elem) { return elem.first; });
    for (auto it = color_keys.begin(); it != color_keys.end(); it++)
    {
        for (auto it2 = it + 1; it2 != color_keys.end(); it2++)
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

void Image::get_line_between_colors(FaceColor c1, FaceColor c2)
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

cv::Mat Image::get_mask(FaceColor color)
{
    return masks_[color];
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
