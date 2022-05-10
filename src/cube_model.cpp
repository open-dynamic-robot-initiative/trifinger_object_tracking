#include <trifinger_object_tracking/cube_model.hpp>

namespace trifinger_object_tracking
{
std::ostream& operator<<(std::ostream& os, const FaceColor& color)
{
    switch (color)
    {
        case FaceColor::RED:
            os << std::string("RED");
            break;
        case FaceColor::GREEN:
            os << std::string("GREEN");
            break;
        case FaceColor::BLUE:
            os << std::string("BLUE");
            break;
        case FaceColor::YELLOW:
            os << std::string("YELLOW");
            break;
        case FaceColor::CYAN:
            os << std::string("CYAN");
            break;
        case FaceColor::MAGENTA:
            os << std::string("MAGENTA");
            break;
        case FaceColor::N_COLORS:
        default:
            throw std::invalid_argument(
                "N_COLORS does not refer to actual color");
    }
    return os;
}

BaseCuboidModel::ConstPtr get_model_by_name(const std::string& name)
{
    if (name == "cube_v1")
    {
        return std::make_shared<const trifinger_object_tracking::CubeV1Model>();
    }
    else if (name == "cube_v2")
    {
        return std::make_shared<const trifinger_object_tracking::CubeV2Model>();
    }
    else if (name == "cube_v3")
    {
        return std::make_shared<const trifinger_object_tracking::CubeV3Model>();
    }
    else if (name == "cuboid_2x2x8_v2")
    {
        return std::make_shared<
            const trifinger_object_tracking::Cuboid2x2x8V2Model>();
    }
    else
    {
        throw std::runtime_error("Invalid cube model name '" + name + "'");
    }
}

}  // namespace trifinger_object_tracking
