#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <armadillo>
#include <boost/algorithm/string.hpp>
#include <trifinger_object_tracking/utils.hpp>

namespace trifinger_object_tracking
{
cv::Mat get_image(const std::string &path, const int idx)
{
    cv::Mat image;
    image = cv::imread(path + "/image_" + std::to_string(idx) + ".jpg", 1);
    if (!image.data)
    {
        std::cout << (path + "/image_" + std::to_string(idx) + ".jpg");
        printf("No image data \n");
        exit(0);
    }
    cv::GaussianBlur(image.clone(), image, cv::Size(5, 5), 0);
    cv::fastNlMeansDenoisingColored(image.clone(), image, 10, 10, 7, 21);
    cv::cvtColor(image, image, CV_BGR2RGB);
    return image;
}

std::vector<std::string> get_directories(const std::string &s)
{
    std::vector<std::string> r;
    for (auto p : fs::recursive_directory_iterator(s))
    {
        if (fs::is_directory(p))
        {
            r.push_back(p.path().string());
            std::cout << p.path().string() << std::endl;
        }
    }
    std::sort(r.begin(), r.end(), comparator);
    return r;
}

bool comparator(const std::string &a, const std::string &b)
{
    std::vector<std::string> split_a, split_b;
    boost::split(split_a, a, [](char c) { return c == '/'; });
    boost::split(split_b, b, [](char c) { return c == '/'; });
    std::string::size_type sz;
    return std::stoi(split_a.back(), &sz) < std::stoi(split_b.back(), &sz);
}

std::vector<cv::Mat> get_images(const std::string &path, bool get_masks)
{
    std::vector<cv::Mat> frames;
    auto files = fs::recursive_directory_iterator(path);

    std::vector<std::string> file_names;
    for (auto p : files)
    {


        int ascii = (int)p.path().string()[37];

        if (ascii >= 48 && ascii <= 57)
        {
            if (get_masks)
            {
                std::cout << p.path().string() << "\n";
                cv::Mat image = cv::imread(p.path().string(), 1);
                if (!image.data)
                {
                    std::cout << (p.path().string());
                    printf("No image data \n");
                    exit(0);
                }
                cv::fastNlMeansDenoisingColored(image, image, 10, 10, 7, 21);
                cv::GaussianBlur(image, image, cv::Size(5, 5), 0);
                cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
                frames.push_back(image);

                cv::Mat temp = image.reshape(1, image.rows * image.cols);
                temp.convertTo(temp, CV_64FC1);
                cv::Mat res;
                remove_if(temp, res, is_zero, true);
                std::cout << res.rows << " " << res.cols << std::endl;
            }
            else
            {
                continue;
            }
        }
        if (get_masks)
        {
            continue;
        }

        file_names.push_back(p.path().string());

    }

    std::swap(file_names[0], file_names[1]);

    for (auto p : file_names)
    {
        std::cout << path << " " << p << "\n";

        cv::Mat image = cv::imread(p, 1);
        if (!image.data)
        {
            std::cout << (p);
            printf("No image data \n");
            exit(0);
        }
        cv::fastNlMeansDenoisingColored(image, image, 10, 10, 7, 21);
        cv::GaussianBlur(image, image, cv::Size(5, 5), 0);
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        frames.push_back(image);
    }

    return frames;
}

std::map<char, std::string> initialize()
{
    std::map<char, std::string> mapper = {{'r', "red"},
                                          {'g', "green"},
                                          {'b', "blue"},
                                          {'y', "yellow"},
                                          {'m', "magenta"},
                                          {'c', "cyan"}};
    return mapper;
}

bool is_zero(const cv::Mat &rc)
{
    return (sum(rc)[0] == 0);
}

// typedef bool (*remove_predicate)(const cv::Mat &rc);

void remove_if(const cv::Mat &mat,
               cv::Mat &res,
               remove_predicate pred,
               bool removeRows = true)
{
    res.release();
    int n = removeRows ? mat.rows : mat.cols;
    for (int i = 0; i < n; i++)
    {
        cv::Mat rc = removeRows ? mat.row(i) : mat.col(i);
        if (pred(rc)) continue;  // remove element
        if (res.empty())
            res = rc;
        else
        {
            if (removeRows)
                vconcat(res, rc, res);
            else
                hconcat(res, rc, res);
        }
    }
}

std::map<std::string, cv::Mat> get_color_masks(
    const std::string &path, std::map<std::string, cv::Mat> dataset)
{
    auto mapper = initialize();
    for (auto p : fs::recursive_directory_iterator(path))
    {
        int ascii = (int)p.path().string()[37];
        if (ascii >= 48 && ascii <= 57)
        {
            std::cout << p.path().string() << "\n";
            std::string color = mapper[p.path().string()[36]];
            cv::Mat image = cv::imread(p.path().string(), 1);
            if (!image.data)
            {
                std::cout << (p);
                printf("No image data \n");
                exit(0);
            }
            cv::Mat image_hsv, concatenated_data, cleaned_data;
            cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV);
            cv::Mat data1 = image.reshape(1, image.rows * image.cols);
            cv::Mat data2 =
                image_hsv.reshape(1, image_hsv.rows * image_hsv.cols);
            data1.convertTo(data1, CV_64FC1);  // bgr
            data2.convertTo(data2, CV_64FC1);  // hsv
            //            cv::hconcat(data1, data2, concatenated_data);

            concatenated_data = data2.clone();
            //            std::cout << "concatenated_data " <<
            //            concatenated_data.rows << " " <<
            //            concatenated_data.cols << std::endl;

            remove_if(concatenated_data,
                      cleaned_data,
                      is_zero,
                      true);  // removing all zero rows
            bool print = false;
            if (print)
            {
                std::cout << "cleaned_data " << cleaned_data.rows << " "
                          << cleaned_data.cols << std::endl;
            }
            if (dataset.find(color) == dataset.end())
            {  // key not found
                dataset[color] = cleaned_data;
            }
            else
            {  // key found
                cv::vconcat(dataset[color], cleaned_data, dataset[color]);
            }
        }
    }
    return dataset;
}

void train_gmm(std::map<std::string, cv::Mat> dataset)
{
    std::map<std::string, int> modes_ = {{"red", 3},
                                         {"green", 3},
                                         {"blue", 4},
                                         {"yellow", 3},
                                         {"magenta", 3},
                                         {"cyan", 4}};

    for (auto &color_data : dataset)
    {
        std::string color = color_data.first;
        arma::mat input_data =
            arma::mat(reinterpret_cast<double *>(color_data.second.data),
                      color_data.second.cols,
                      color_data.second.rows);

        arma::gmm_diag model;
        bool status = model.learn(input_data,
                                  modes_[color],
                                  arma::maha_dist,
                                  arma::random_subset,
                                  100,
                                  1000,
                                  1e-6,
                                  true);
        if (status == false)
        {
            std::cout << "learning failed" << std::endl;
        }
        model.means.print("means:");
        std::string name = "../data/" + color + "_diag_hsv.gmm";
        model.save(name);
        std::cout << color << " done\n";
    }
}
}  // namespace trifinger_object_tracking