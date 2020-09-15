#pragma once
#include <math.h>
#include <armadillo>
#include <chrono>
#include <future>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>

namespace trifinger_object_tracking
{
struct ColorBounds
{
    std::string name;
    int lower[3];
    int upper[3];
};

class Image
{
private:
    // private variables
    cv::Mat image_, image_hsv_, image_bgr_;
    std::string model_directory_;
    std::map<std::string, ColorBounds> colors_;
    //! individual color segment mask
    std::map<std::string, cv::Mat> masks_;
    std::map<std::string, std::vector<int>> pixel_idx_;
    //! pixel coordinates of the region of interest
    std::map<std::string, std::vector<cv::Point>> pixel_dataset_;
    //! total pixels with a particular color
    std::map<std::string, int> color_count_;

    struct cmp
    {  // Declaring a set that will store the std::pairs using the comparator
       // logic
        bool operator()(std::pair<std::string, int> elem1,
                        std::pair<std::string, int> elem2)
        {
            return elem1.second > elem2.second;
        }
    };

    std::set<std::pair<std::string, int>, cmp> dominant_colors_;

    struct cmp2
    {  // Declaring a set that will store the std::pairs using the comparator
       // logic
        bool operator()(std::pair<int, int> elem1, std::pair<int, int> elem2)
        {
            return elem1.second > elem2.second;
        }
    };

    std::set<std::pair<int, int>, cmp2> label_count_;

public:
    // constructor
    Image(cv::Mat, std::string model_directory);

    // member functions
    void set_color_bounds();

    void initialise_object_model();

    void create_mask(const std::string &);

    cv::Mat get_mask(const std::string &color_name);

    void start();

    void startSingleThread();

    void create_pixel_dataset(const std::string &);

    void pipeline(const std::string &);

    void find_dominant_colors(const int);

    void show();

    void print_pixels();

    void remove_outliers();

    std::vector<std::pair<std::string, std::string>> make_valid_combinations();

    void get_line_between_colors(const std::string &, const std::string &);

    cv::Mat get_segmented_image();

    cv::Mat get_segmented_image_wout_outliers();

    cv::Mat get_image_lines();

    cv::Mat get_image();

    void gmm_mask();

    bool denoise();

    void clean_mask(const std::string &color);

    int getNthElement(int n);

    void gmm_lop_p(const std::string &color, const int);

    void arg_max(const int idx);

    void create_final_mask(const std::string &color);

    void cuda_gmm();

    void start_timer();

    void finish_timer(bool verbose, const std::string &message);

    void print_time_taken(const std::string &message);

    void initialize_variables();  // initialized masks_, pixel_idx_ and
                                  // threshold_ variables
    void gmm_isolated();

    // member variables
    std::map<std::string, float> threshold_;
    std::map<std::pair<std::string, std::string>, std::pair<float, float>>
        lines_;
    std::map<std::pair<std::string, std::string>, std::pair<int, int>>
        object_model_;
    std::map<int, std::string> idx2color_;
    arma::mat gmm_result_ = arma::mat(6, 388800, arma::fill::zeros);
    arma::mat input_data;
    std::chrono::high_resolution_clock::time_point start_, finish_;
};

}  // namespace trifinger_object_tracking
