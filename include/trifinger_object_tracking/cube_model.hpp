/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <cstdint>
#include <map>

namespace trifinger_object_tracking
{
enum class FaceColor
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA
};

std::ostream& operator<<(std::ostream& os, const FaceColor& color);


class CubeModel
{
public:
    std::map<std::pair<FaceColor, FaceColor>, std::pair<int, int>>
        object_model_;

    CubeModel()
    {
        initialise_object_model();
    }

    static std::array<FaceColor, 6> get_colors()
    {
        return {FaceColor::RED,
                FaceColor::GREEN,
                FaceColor::BLUE,
                FaceColor::YELLOW,
                FaceColor::CYAN,
                FaceColor::MAGENTA};
    }

    static std::string get_color_name(FaceColor color)
    {
        switch (color)
        {
            case FaceColor::RED:
                return "red";
            case FaceColor::GREEN:
                return "green";
            case FaceColor::BLUE:
                return "blue";
            case FaceColor::YELLOW:
                return "yellow";
            case FaceColor::CYAN:
                return "cyan";
            case FaceColor::MAGENTA:
                return "magenta";
        }
    }

    static std::array<int, 3> get_rgb(FaceColor color)
    {
        switch (color)
        {
            case FaceColor::RED:
                return {255, 0, 0};
            case FaceColor::GREEN:
                return {0, 255, 0};
            case FaceColor::BLUE:
                return {0, 0, 255};
            case FaceColor::YELLOW:
                return {255, 255, 0};
            case FaceColor::CYAN:
                return {0, 255, 255};
            case FaceColor::MAGENTA:
                return {255, 0, 255};
        }
    }

    static std::array<int, 3> get_hsv(FaceColor color)
    {
        switch (color)
        {
            case FaceColor::RED:
                return {0, 255, 255};
            case FaceColor::GREEN:
                return {60, 255, 255};
            case FaceColor::BLUE:
                return {120, 255, 255};
            case FaceColor::YELLOW:
                return {30, 255, 255};
            case FaceColor::CYAN:
                return {90, 255, 255};
            case FaceColor::MAGENTA:
                return {150, 255, 255};
        }
    }

private:
    void initialise_object_model()
    {
        object_model_[{FaceColor::YELLOW, FaceColor::CYAN}] = {0, 1};
        object_model_[{FaceColor::YELLOW, FaceColor::MAGENTA}] = {0, 2};
        object_model_[{FaceColor::YELLOW, FaceColor::GREEN}] = {1, 3};
        object_model_[{FaceColor::YELLOW, FaceColor::BLUE}] = {2, 3};
        object_model_[{FaceColor::BLUE, FaceColor::GREEN}] = {3, 7};
        object_model_[{FaceColor::CYAN, FaceColor::GREEN}] = {1, 5};
        object_model_[{FaceColor::CYAN, FaceColor::MAGENTA}] = {0, 4};
        object_model_[{FaceColor::BLUE, FaceColor::MAGENTA}] = {2, 6};
        object_model_[{FaceColor::RED, FaceColor::MAGENTA}] = {4, 6};
        object_model_[{FaceColor::RED, FaceColor::CYAN}] = {4, 5};
        object_model_[{FaceColor::RED, FaceColor::GREEN}] = {5, 7};
        object_model_[{FaceColor::RED, FaceColor::BLUE}] = {6, 7};

        // for each color-pair (C1, C2), add also (C2, C1) to the map
        for (auto& i : object_model_)
        {
            object_model_[std::make_pair(i.first.second, i.first.first)] =
                i.second;
        }
    }
};

}  // namespace trifinger_object_tracking
