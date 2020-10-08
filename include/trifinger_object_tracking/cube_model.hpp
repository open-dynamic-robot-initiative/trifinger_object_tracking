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
enum CubeFace
{
    FACE_0 = 0,
    FACE_1,
    FACE_2,
    FACE_3,
    FACE_4,
    FACE_5
};

enum FaceColor
{
    // IMPORTANT: Do not change order here!
    RED = 0,
    GREEN,
    BLUE,
    CYAN,
    MAGENTA,
    YELLOW,
    N_COLORS  // TODO remove
};

std::ostream& operator<<(std::ostream& os, const FaceColor& color);

class CubeModel
{
public:
    struct Edge
    {
        // Indices of the cube corners that belong to that edge
        unsigned int c1;
        unsigned int c2;
    };

    static constexpr unsigned int N_FACES = 6;

    static constexpr float WIDTH = 0.0652;
    static constexpr float HALF_WIDTH = WIDTH / 2.0;

#if CUBE_VERSION == 1
    // Colour mapping of cube v1.  Red, green, blue faces are pointing in x, y,
    // z directions.
    static constexpr std::array<CubeFace, N_FACES> map_color_to_face = {
        CubeFace::FACE_1,  // RED
        CubeFace::FACE_2,  // GREEN
        CubeFace::FACE_0,  // BLUE
        CubeFace::FACE_5,  // CYAN
        CubeFace::FACE_4,  // MAGENTA
        CubeFace::FACE_3   // YELLOW
    };
#elif CUBE_VERSION == 2
    // Colour mapping of cube v2.  Red, green, blue faces are pointing in x, y,
    // z directions.
    static constexpr std::array<CubeFace, N_FACES> map_color_to_face = {
        CubeFace::FACE_1,  // RED
        CubeFace::FACE_2,  // GREEN
        CubeFace::FACE_0,  // BLUE
        CubeFace::FACE_5,  // CYAN
        CubeFace::FACE_3,  // MAGENTA
        CubeFace::FACE_4   // YELLOW
    };
#endif

    // TODO make this nice and const
    std::map<std::pair<CubeFace, CubeFace>, Edge> edges;

    static constexpr std::array<std::array<unsigned int, 4>, N_FACES>
        face_corner_indices = {{{0, 1, 2, 3},
                                {4, 5, 1, 0},
                                {5, 6, 2, 1},
                                {6, 7, 3, 2},
                                {7, 4, 0, 3},
                                {7, 6, 5, 4}}};

    //! Cube corner positions in cube frame
    static constexpr float cube_corners[8][4] = {
        {+HALF_WIDTH, -HALF_WIDTH, +HALF_WIDTH, 1},
        {+HALF_WIDTH, +HALF_WIDTH, +HALF_WIDTH, 1},
        {-HALF_WIDTH, +HALF_WIDTH, +HALF_WIDTH, 1},
        {-HALF_WIDTH, -HALF_WIDTH, +HALF_WIDTH, 1},
        {+HALF_WIDTH, -HALF_WIDTH, -HALF_WIDTH, 1},
        {+HALF_WIDTH, +HALF_WIDTH, -HALF_WIDTH, 1},
        {-HALF_WIDTH, +HALF_WIDTH, -HALF_WIDTH, 1},
        {-HALF_WIDTH, -HALF_WIDTH, -HALF_WIDTH, 1}};

    static constexpr float face_normal_vectors[6][3] = {
        {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}};

    CubeModel()
    {
        initialise_edges();
    }

    static std::array<FaceColor, 6> get_colors()
    {
        return {FaceColor::RED,
                FaceColor::GREEN,
                FaceColor::BLUE,
                FaceColor::CYAN,
                FaceColor::MAGENTA,
                FaceColor::YELLOW};
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
            case FaceColor::N_COLORS:
            default:
                throw std::invalid_argument(
                    "N_COLORS does not refer to actual color");
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
            case FaceColor::N_COLORS:
            default:
                throw std::invalid_argument(
                    "N_COLORS does not refer to actual color");
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
            case FaceColor::N_COLORS:
            default:
                throw std::invalid_argument(
                    "N_COLORS does not refer to actual color");
        }
    }

    //! Maps each color to the indices of the corresponding cube corners.
    static std::array<unsigned int, 4> get_face_corner_indices(FaceColor color)
    {
        const CubeFace face = map_color_to_face[color];
        return face_corner_indices[face];
    }

private:
    void initialise_edges()
    {
        edges[{CubeFace::FACE_0, CubeFace::FACE_1}] = {0, 1};
        edges[{CubeFace::FACE_0, CubeFace::FACE_2}] = {1, 2};
        edges[{CubeFace::FACE_0, CubeFace::FACE_3}] = {2, 3};
        edges[{CubeFace::FACE_0, CubeFace::FACE_4}] = {3, 0};

        edges[{CubeFace::FACE_5, CubeFace::FACE_1}] = {4, 5};
        edges[{CubeFace::FACE_5, CubeFace::FACE_2}] = {5, 6};
        edges[{CubeFace::FACE_5, CubeFace::FACE_3}] = {6, 7};
        edges[{CubeFace::FACE_5, CubeFace::FACE_4}] = {7, 4};

        edges[{CubeFace::FACE_1, CubeFace::FACE_2}] = {1, 5};
        edges[{CubeFace::FACE_2, CubeFace::FACE_3}] = {2, 6};
        edges[{CubeFace::FACE_3, CubeFace::FACE_4}] = {3, 7};
        edges[{CubeFace::FACE_4, CubeFace::FACE_1}] = {0, 4};

        // for each face-pair (F1, F2), add also (F2, F1) to the map
        for (auto& i : edges)
        {
            edges[std::make_pair(i.first.second, i.first.first)] = i.second;
        }
    }
};

}  // namespace trifinger_object_tracking
