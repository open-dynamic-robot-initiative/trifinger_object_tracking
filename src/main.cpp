#include <chrono>
#include <fstream>
#include <ostream>
#include <thread>

#include <trifinger_object_tracking/cube_model.hpp>
#include <trifinger_object_tracking/cv_sub_images.hpp>
#include <trifinger_object_tracking/line_detector.hpp>
#include <trifinger_object_tracking/pose_detector.hpp>
#include <trifinger_object_tracking/utils.hpp>

int debug = 1;
int cols_plot = 5;

using namespace trifinger_object_tracking;

[[deprecated]] std::vector<cv::Mat> get_images(const std::string &path,
                                               bool get_masks = false) {
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
                // cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
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
        // cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        frames.push_back(image);
    }

    return frames;
}

int main(int argc, char **argv)
{
    std::string data_dir = "../data/cube_dataset_real_cube";
    if (argc > 1)
    {
        data_dir = argv[1];
    }
    std::vector<std::string> camera_data =
        trifinger_object_tracking::get_directories(data_dir);

    // set true if you want to train gmm models for each color
    bool generate_gmm_dataset = false;

    // Generate dataset for GMM model for each color and train it
    if (generate_gmm_dataset)
    {
        std::map<std::string, cv::Mat> training_dataset_for_gmm;
        for (std::string folder_path : camera_data)
        {
            training_dataset_for_gmm =
                trifinger_object_tracking::get_color_masks(
                    folder_path, training_dataset_for_gmm);
        }
        trifinger_object_tracking::train_gmm(training_dataset_for_gmm);
    }
    else  // Use the trained GMM model to make predictions
    {
        trifinger_object_tracking::CubeModel cube_model;
        trifinger_object_tracking::LineDetector line_detector(cube_model,
                                                              "../data");
        trifinger_object_tracking::PoseDetector pose(cube_model);

        trifinger_object_tracking::CvSubImages subplot(
            cv::Size(720, 540), 3, 5);
        cv::namedWindow("debug", cv::WINDOW_NORMAL);
        cv::resizeWindow(
            "debug", subplot.get_image().cols, subplot.get_image().rows);

        // for saving to a video file
        int frame_width = 1815;
        int frame_height = 820;
        cv::Size frame_size(frame_width, frame_height);
        int frames_per_second = 1;
        cv::VideoWriter oVideoWriter(
            "./video_3.avi",
            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
            frames_per_second,
            frame_size,
            true);

        // If the VideoWriter object is not initialized successfully, exit the
        // program
        if (oVideoWriter.isOpened() == false)
        {
            std::cout << "Cannot save the video to a file" << std::endl;
            std::cin.get();  // wait for any key press
            return -1;
        }

        int until = 0;
        for (std::string folder_path : camera_data)
        {
            until++;
            //            if (until == 5) { break;}
            // getting frames from three cameras
            //            folder_path = "../data/cube_dataset_real_cube/0003/";
            std::vector<cv::Mat> frames = get_images(folder_path);

            // sending frames color segmentation
            auto start = std::chrono::high_resolution_clock::now();

            std::array<std::map<trifinger_object_tracking::ColorPair,
                                trifinger_object_tracking::Line>,
                       3>
                lines;

            int i = 0;
            for (auto &image : frames)
            {
                lines[i] = line_detector.detect_lines(image.clone());

                if (debug == 1)
                {
                    subplot.set_subimg(line_detector.get_image(), i, 0);
                    subplot.set_subimg(
                        line_detector.get_segmented_image(), i, 1);
                    subplot.set_subimg(
                        line_detector.get_segmented_image_wout_outliers(),
                        i,
                        2);
                    subplot.set_subimg(line_detector.get_image_lines(), i, 3);
                }
                i++;
            }

            auto finish = std::chrono::high_resolution_clock::now();
            std::cout << "Segmentation took "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(
                             finish - start)
                             .count()
                      << " milliseconds\n";
            std::cout << "Segmentation complete"
                      << "\n";

            std::cout << "\n@#$@#$@#$@#$@#$@#$\n\n";
            int print_lines = 1;  // 0 = False
            if (print_lines == 1)
            {
                for (const auto &image_lines : lines)
                {
                    std::cout << "Lines\n";
                    for (const auto &l : image_lines)
                    {
                        std::cout << l.first.first << " " << l.first.second
                                  << " " << l.second.a << " " << l.second.b
                                  << std::endl;  // prints color_pairs and
                                                 // slope-intercept for the
                                                 // separating line
                    }
                }
            }

            // TODO: remove the following part
            if (true)
            {
                std::fstream file;
                std::string word, filename = "../data/lines/" +
                                             std::to_string(until) + ".txt";
                file.open(filename.c_str());
                std::cout << filename << std::endl;
                int iii = -1;
                FaceColor c1, c2;
                float f1, f2;
                int counter;

                std::map<std::string, FaceColor> color_from_string;
                color_from_string["red"] = FaceColor::RED;
                color_from_string["green"] = FaceColor::GREEN;
                color_from_string["blue"] = FaceColor::BLUE;
                color_from_string["yellow"] = FaceColor::YELLOW;
                color_from_string["cyan"] = FaceColor::CYAN;
                color_from_string["magenta"] = FaceColor::MAGENTA;

                while (file >> word)
                {  // take word and print
                    std::cout << word << std::endl;
                    if (word == "new")
                    {
                        counter = 0;
                        iii++;
                        if (iii < 3)
                        {
                            lines[iii].clear();
                        }
                    }
                    else
                    {
                        switch (counter)
                        {
                            case 0:
                                c1 = color_from_string.at(word);
                                break;
                            case 1:
                                c2 = color_from_string.at(word);
                                break;
                            case 2:
                                f1 = std::atof(word.c_str());
                                break;
                            case 3:
                                f2 = std::atof(word.c_str());
                                lines[iii][std::make_pair(c1, c2)] =
                                    Line(f1, f2);
                                counter = -1;
                                break;
                            default:
                                break;
                        }
                        counter++;
                    }
                }
            }

            pose.find_pose(lines);

            std::cout << "Pose detected\n";

            if (debug == 1)
            {
                auto projected_cube_corners = pose.get_projected_points();
                for (int i = 0; i < frames.size(); i++)
                {
                    std::vector<cv::Point2f> imgpoints =
                        projected_cube_corners[i];
                    cv::Mat poseimg = frames[i].clone();
                    // draw the cube edges in the image
                    for (auto &it : cube_model.object_model_)
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
            }

            cv::Mat debug_img = subplot.get_image();

            // scale down debug image by factor 2, otherwise it is too big
            cv::Mat rescaled_debug_img;
            cv::resize(debug_img,
                       rescaled_debug_img,
                       cv::Size(debug_img.cols / 2, debug_img.rows / 2));

            // write the video frame to the file
            oVideoWriter.write(rescaled_debug_img);

            int show = 1;
            if (show == 1)
            {
                cv::imshow("debug", rescaled_debug_img);
                char key = cv::waitKey(0);

                if (key == 'q')
                {
                    break;
                }
            }
        }
        // Flush and close the video file
        oVideoWriter.release();
    }

    return 0;
}
