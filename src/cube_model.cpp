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
}  // namespace trifinger_object_tracking
