#include <chrono>
#include <thread>
#include <trifinger_object_tracking/image.hpp>
#include <trifinger_object_tracking/pose.hpp>
#include <trifinger_object_tracking/utils.hpp>
#include <trifinger_object_tracking/cv_sub_images.hpp>
#include <ostream>
#include <fstream>


int debug = 1;
int cols_plot = 5;


int main(int argc, char **argv)
{
    std::string data_dir = "../data/cube_dataset_real_cube";
    if (argc > 1)
    {
        data_dir = argv[1];
    }
    std::vector<std::string> camera_data =
        trifinger_object_tracking::get_directories(data_dir);

    bool generate_gmm_dataset =
        false;  // set true if you want to train gmm models for each color

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

    // Use the trained GMM model to make predictions
    if (!generate_gmm_dataset)
    {
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
        cv::VideoWriter oVideoWriter("./video_3.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                      frames_per_second, frame_size, true);

        //If the VideoWriter object is not initialized successfully, exit the program
        if (oVideoWriter.isOpened() == false)
        {
            std::cout << "Cannot save the video to a file" << std::endl;
            std::cin.get(); //wait for any key press
            return -1;
        }

        int until = 0;
        for (std::string folder_path : camera_data)
        {
            until++;
//            if (until == 5) { break;}
            // getting frames from three cameras
//            folder_path = "../data/cube_dataset_real_cube/0003/";
            std::vector<cv::Mat> frames =
                trifinger_object_tracking::get_images(folder_path);

            // sending frames color segmentation
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<trifinger_object_tracking::Image> images;
            int thread_type = 0;  // 1 = multi-threaded

            if (thread_type == 0)
            {
                int i = 0;
                for (auto &image : frames)
                {
                    trifinger_object_tracking::Image obj(image.clone());
                    obj.startSingleThread();
                    images.push_back(obj);

                    if (debug == 1)
                    {
                        subplot.set_subimg(images[i].get_image(), i, 0);
                        subplot.set_subimg(
                            images[i].get_segmented_image(), i, 1);
                        subplot.set_subimg(
                            images[i].get_segmented_image_wout_outliers(),
                            i,
                            2);
                        subplot.set_subimg(images[i].get_image_lines(), i, 3);
                    }
                    i++;
                }
            }
            else
            {
                std::vector<std::thread> thread_vector;
                for (auto &image : frames)
                {
                    trifinger_object_tracking::Image obj(image);
                    images.push_back(obj);
                    std::thread th(
                        &trifinger_object_tracking::Image::startSingleThread,
                        images.back());
                    thread_vector.push_back(move(th));
                }

                // waiting for threads to finish
                for (std::thread &th : thread_vector)
                {
                    if (th.joinable()) th.join();
                }
            }
            auto finish = std::chrono::high_resolution_clock::now();
            std::cout << "Segmentation took "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(
                             finish - start)
                             .count()
                      << " milliseconds\n";
            std::cout << "Segmentation complete"
                      << "\n";
            int display = 0;  // 0 = False
            if (display == 1)
            {
                for (auto t : images)
                {
                    t.show();
                }
            }

            std::cout << "\n@#$@#$@#$@#$@#$@#$\n\n";
            int lines = 1;  // 0 = False
            if (lines == 1)
            {
                for (auto t : images)
                {
                    std::cout << "Lines\n";
                    for (auto &l : t.lines_)
                    {
                        std::cout << l.first.first << " " << l.first.second
                                  << " " << l.second.first << " "
                                  << l.second.second
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
                std::string word, filename = "../data/lines/" + std::to_string(until) + ".txt";
                file.open(filename.c_str());
                std::cout << filename << std::endl;
                int iii = -1;
                std::string c1,c2;
                float f1,f2;
                int counter;
                while(file >> word) { //take word and print
                    std::cout << word << std::endl;
                    if (word == "new")
                    {
                        counter = 0;
                        iii++;
                        if (iii < 3)
                        {
                            images[iii].lines_.clear();
                        }
                    }
                    else
                    {
                        switch(counter)
                        {
                            case 0:
                                c1 = word;
                                break;
                            case 1:
                                c2 = word;
                                break;
                            case 2:
                                f1 = std::atof(word.c_str());
                                break;
                            case 3:
                                f2 = std::atof(word.c_str());
                                images[iii].lines_[std::make_pair(c1, c2)] = {f1, f2};
                                counter = -1;
                                break;
                            default:
                                break;
                        }
                        counter++;
                    }
                }

            }


            // Pose Detection from below
            trifinger_object_tracking::Pose pose(images);
            pose.find_pose();

            std::cout << "Pose detected\n";

            if (debug == 1)
            {
                for (int i = 0; i < images.size(); i++)
                {
                    std::vector<cv::Point2f> imgpoints =
                        pose.projected_points_[i];
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
            }

            cv::Mat debug_img = subplot.get_image();

            cv::cvtColor(debug_img, debug_img, cv::COLOR_RGB2BGR);

            // scale down debug image by factor 2, otherwise it is too big
            cv::Mat rescaled_debug_img;
            cv::resize(debug_img,
                       rescaled_debug_img,
                       cv::Size(debug_img.cols / 2, debug_img.rows / 2));

            //write the video frame to the file
            oVideoWriter.write(rescaled_debug_img);

            int show = 0;
            if (show==1)
            {
                cv::imshow("debug", rescaled_debug_img);
                char key = cv::waitKey(0);

                if (key == 'q')
                {
                    break;
                }
            }


        }
        //Flush and close the video file
        oVideoWriter.release();
    }

    return 0;
}
