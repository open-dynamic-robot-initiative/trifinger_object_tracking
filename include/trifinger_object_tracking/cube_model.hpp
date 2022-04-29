/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "color_model.hpp"

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

/**
 * @brief Base class for all cuboid object models
 */
class BaseCuboidModel
{
public:
    typedef std::shared_ptr<BaseCuboidModel> Ptr;
    typedef std::shared_ptr<const BaseCuboidModel> ConstPtr;

    //! @brief Represents a cuboid edge, defined by its two corner points.
    struct Edge
    {
        //! @brief Index of the first cuboid corner that belong to the edge.
        unsigned int c1;
        //! @brief Index of the second cuboid corner that belong to the edge.
        unsigned int c2;
    };

    //! @brief Number of faces of the cuboid.
    static constexpr unsigned int N_FACES = 6;

    typedef std::array<std::array<float, 4>, 8> corners_t;  // FIXME name
    typedef std::array<CubeFace, N_FACES> CubeFaceArray;

    //! @brief Get name of the model.
    virtual std::string get_name() const = 0;

    //! @brief Get homogeneous coordinates (x, y, z, 1) of the cube corners.
    virtual corners_t get_corners() const = 0;

    //! @brief Get the cuboid face that has the specified colour.
    virtual CubeFace map_color_to_face(FaceColor color) const = 0;

    //! @brief Get the colour model that is used for this object.
    virtual ColorModel get_color_model() const = 0;

    // TODO make this nice and const
    std::map<std::pair<CubeFace, CubeFace>, Edge> edges;

    //! For each cuboid face the indices of the corresponding corners.
    static constexpr std::array<std::array<unsigned int, 4>, N_FACES>
        face_corner_indices = {{{0, 1, 2, 3},
                                {4, 5, 1, 0},
                                {5, 6, 2, 1},
                                {6, 7, 3, 2},
                                {7, 4, 0, 3},
                                {7, 6, 5, 4}}};

    //! Normal vectors of all cuboid faces.
    static constexpr float face_normal_vectors[6][3] = {
        {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}};

    BaseCuboidModel()
    {
        initialise_edges();
    }

    //! @brief Get face colours. The index in the list refers to the face index.
    static std::array<FaceColor, 6> get_colors()
    {
        return {FaceColor::RED,
                FaceColor::GREEN,
                FaceColor::BLUE,
                FaceColor::CYAN,
                FaceColor::MAGENTA,
                FaceColor::YELLOW};
    }

    //! @brief Get name of the given colour.
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

    /**
     * @brief Get RGB value of the given colour.
     *
     * Note that the RGB values returned by this function are only meant for
     * visualisation purposes and do not necessarily represent the actual shade
     * of the colour on the given object (e.g. the value returned for red is
     * always (255, 0, 0), independent of the object.
     */
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

    /**
     * @brief Get HSV value of the given colour.
     *
     * Like @ref get_rgb() this is only for visualisation purposes and does not
     * attempt to match the actual colour of the real object.
     */
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
    std::array<unsigned int, 4> get_face_corner_indices(FaceColor color) const
    {
        const CubeFace face = map_color_to_face(color);
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

/**
 * @brief Base model for all 65mm cubes.
 */
class BaseCubeModel : public virtual BaseCuboidModel
{
public:
    static constexpr float WIDTH = 0.0652;
    static constexpr float HALF_WIDTH = WIDTH / 2.0;

    //! Cube corner positions in cube frame
    static constexpr corners_t cube_corners{
        {{+HALF_WIDTH, -HALF_WIDTH, +HALF_WIDTH, 1},
         {+HALF_WIDTH, +HALF_WIDTH, +HALF_WIDTH, 1},
         {-HALF_WIDTH, +HALF_WIDTH, +HALF_WIDTH, 1},
         {-HALF_WIDTH, -HALF_WIDTH, +HALF_WIDTH, 1},
         {+HALF_WIDTH, -HALF_WIDTH, -HALF_WIDTH, 1},
         {+HALF_WIDTH, +HALF_WIDTH, -HALF_WIDTH, 1},
         {-HALF_WIDTH, +HALF_WIDTH, -HALF_WIDTH, 1},
         {-HALF_WIDTH, -HALF_WIDTH, -HALF_WIDTH, 1}}};

    corners_t get_corners() const override
    {
        return cube_corners;
    }
};

/**
 * @brief Model for 65mm cube version 1.
 *
 * @image html images/cube_v1.jpg
 *
 * @verbatim embed:rst:leading-asterisk
 * See :ref:`object_model` for more information on the object model.
 * @endverbatim
 */
class CubeV1Model : public BaseCubeModel
{
public:
    std::string get_name() const override
    {
        return "cube_v1";
    }

    CubeFace map_color_to_face(FaceColor color) const override
    {
        // Colour mapping of cube v1.  Red, green, blue faces are pointing in x,
        // y, z directions.
        static constexpr std::array<CubeFace, N_FACES> color_to_face = {
            CubeFace::FACE_1,  // RED
            CubeFace::FACE_2,  // GREEN
            CubeFace::FACE_0,  // BLUE
            CubeFace::FACE_5,  // CYAN
            CubeFace::FACE_4,  // MAGENTA
            CubeFace::FACE_3   // YELLOW
        };

        return color_to_face[color];
    }

    ColorModel get_color_model() const override
    {
        return ColorModel::CUBE_V1;
    }
};

// color order is the same for V2 and V3 objects, so use a common base class for
// this
class CubeV2ColorOrderBase : public virtual BaseCuboidModel
{
public:
    CubeFace map_color_to_face(FaceColor color) const override
    {
        // Colour mapping of cube v2.  Red, green, blue faces are pointing in x,
        // y, z directions.
        static constexpr std::array<CubeFace, N_FACES> color_to_face = {
            CubeFace::FACE_1,  // RED
            CubeFace::FACE_2,  // GREEN
            CubeFace::FACE_0,  // BLUE
            CubeFace::FACE_5,  // CYAN
            CubeFace::FACE_3,  // MAGENTA
            CubeFace::FACE_4   // YELLOW
        };

        return color_to_face[color];
    }
};

/**
 * @brief Model for 65mm cube version 2.
 *
 * @image html images/cube_v2.jpg
 *
 * @verbatim embed:rst:leading-asterisk
 * See :ref:`object_model` for more information on the object model.
 * @endverbatim
 */
class CubeV2Model : public BaseCubeModel, CubeV2ColorOrderBase
{
public:
    std::string get_name() const override
    {
        return "cube_v2";
    }

    ColorModel get_color_model() const override
    {
        return ColorModel::CUBE_V2;
    }
};

/**
 * @brief Model for 65mm cube version 3.
 *
 * @image html images/cube_v3.jpg
 *
 * @verbatim embed:rst:leading-asterisk
 * See :ref:`object_model` for more information on the object model.
 * @endverbatim
 */
class CubeV3Model : public BaseCubeModel, CubeV2ColorOrderBase
{
public:
    std::string get_name() const override
    {
        return "cube_v3";
    }

    ColorModel get_color_model() const override
    {
        throw std::runtime_error("Not implemented");
    }
};

/**
 * @brief Model for the 2x2x8 cm cuboid version 2.
 *
 * @image html images/cuboid_2x2x8.jpg
 *
 * @verbatim embed:rst:leading-asterisk
 * See :ref:`object_model` for more information on the object model.
 * @endverbatim
 */
class Cuboid2x2x8V2Model : public virtual BaseCuboidModel, CubeV2ColorOrderBase
{
public:
    static constexpr float LENGTH = 0.08;
    static constexpr float LONG_HALF_WIDTH = LENGTH / 2.0;
    static constexpr float WIDTH = 0.02;
    static constexpr float SHORT_HALF_WIDTH = WIDTH / 2.0;

    //! Cube corner positions in cube frame
    static constexpr corners_t cube_corners = {
        {{+SHORT_HALF_WIDTH, -LONG_HALF_WIDTH, +SHORT_HALF_WIDTH, 1},
         {+SHORT_HALF_WIDTH, +LONG_HALF_WIDTH, +SHORT_HALF_WIDTH, 1},
         {-SHORT_HALF_WIDTH, +LONG_HALF_WIDTH, +SHORT_HALF_WIDTH, 1},
         {-SHORT_HALF_WIDTH, -LONG_HALF_WIDTH, +SHORT_HALF_WIDTH, 1},
         {+SHORT_HALF_WIDTH, -LONG_HALF_WIDTH, -SHORT_HALF_WIDTH, 1},
         {+SHORT_HALF_WIDTH, +LONG_HALF_WIDTH, -SHORT_HALF_WIDTH, 1},
         {-SHORT_HALF_WIDTH, +LONG_HALF_WIDTH, -SHORT_HALF_WIDTH, 1},
         {-SHORT_HALF_WIDTH, -LONG_HALF_WIDTH, -SHORT_HALF_WIDTH, 1}}};

    std::string get_name() const override
    {
        return "cuboid_2x2x8_v2";
    }

    corners_t get_corners() const override
    {
        return cube_corners;
    }

    ColorModel get_color_model() const override
    {
        return ColorModel::CUBOID_V2;
    }
};

/**
 * @brief Get object model instance by name
 *
 * @param name Name of the model type.  One of cube_v1, cube_v2, cube_v3,
 *      cuboid_2x2x8_v2 (same names as returned by @ref BaseCuboidModel::get_name()).
 *
 * @return Instance of the specified model.
 */
BaseCuboidModel::ConstPtr get_model_by_name(const std::string& name);
// TODO: unit test for get_model_by_name()

}  // namespace trifinger_object_tracking
