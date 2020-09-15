#pragma once
#include <math.h>
#include <armadillo>
#include <chrono>
#include <future>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>

#include <trifinger_object_tracking/cube_model.hpp>

namespace trifinger_object_tracking
{
struct ColorBounds
{
    int lower[3];
    int upper[3];
};

class Image
{
private:
    // private variables
    cv::Mat image_, image_hsv_, image_bgr_;

    std::array<ColorBounds, FaceColor::N_COLORS> color_bounds_;

    //! individual color segment mask
    std::map<FaceColor, cv::Mat> masks_;
    std::map<FaceColor, std::vector<int>> pixel_idx_;
    //! pixel coordinates of the region of interest
    std::map<FaceColor, std::vector<cv::Point>> pixel_dataset_;
    //! total pixels with a particular color
    std::map<FaceColor, int> color_count_;

    std::array<arma::gmm_diag, FaceColor::N_COLORS> segmentation_models_;

    std::map<FaceColor, float> threshold_;
    std::map<int, FaceColor> idx2color_;
    std::chrono::high_resolution_clock::time_point start_, finish_;


    struct cmp
    {  // Declaring a set that will store the std::pairs using the comparator
       // logic
        bool operator()(std::pair<FaceColor, int> elem1,
                        std::pair<FaceColor, int> elem2)
        {
            return elem1.second > elem2.second;
        }
    };

    std::set<std::pair<FaceColor, int>, cmp> dominant_colors_;

    struct cmp2
    {  // Declaring a set that will store the std::pairs using the comparator
       // logic
        bool operator()(std::pair<int, int> elem1, std::pair<int, int> elem2)
        {
            return elem1.second > elem2.second;
        }
    };

    std::set<std::pair<int, int>, cmp2> label_count_;


    void set_color_bounds();
    void load_segmentation_models(const std::string &model_directory);

public:
    // constructor
    Image(cv::Mat, const std::string &model_directory);

    // member functions

    cv::Mat get_mask(FaceColor color);

    void run_line_detection();

    void create_pixel_dataset(FaceColor color);

    void find_dominant_colors(const int);

    void show();

    void print_pixels();

    void remove_outliers();

    std::vector<std::pair<FaceColor, FaceColor>> make_valid_combinations();

    void get_line_between_colors(FaceColor color1, FaceColor color2);

    cv::Mat get_segmented_image();

    cv::Mat get_segmented_image_wout_outliers();

    cv::Mat get_image_lines();

    cv::Mat get_image();

    void gmm_mask();

    bool denoise();

    void clean_mask(FaceColor color);

    int getNthElement(int n);

    void create_final_mask(FaceColor color);

    void cuda_gmm();

    void start_timer();

    void finish_timer(bool verbose, const std::string &message);

    void print_time_taken(const std::string &message);

    void initialize_variables();  // initialized masks_, pixel_idx_ and
                                  // threshold_ variables
    void gmm_isolated();

    // member variables
    std::map<std::pair<FaceColor, FaceColor>, std::pair<float, float>> lines_;

    CubeModel cube_model_;
};

}  // namespace trifinger_object_tracking
