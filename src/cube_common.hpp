#pragma once
#include <stdexcept>
#include <string>

constexpr int Rank = 3;
constexpr int RoundCount = 8;
constexpr int SurfaceCount = 6;
enum Surface { Front,
               Back,
               Left,
               Right,
               Up,
               Down };
//  0    1    2
//  7   mid   3
//  6    5    4
enum RotationPosition { UpLeft = 0,
                        UpMiddle,
                        UpRight,
                        MiddleRight,
                        DownRight,
                        DownMiddle,
                        DownLeft,
                        MiddleLeft = 7 };
struct SurfaceWithRotationOffset {
    Surface surface;
    int rotation_offset;  // positive: clockwise

    SurfaceWithRotationOffset(Surface surface, int rotation_offset)
        : surface(surface), rotation_offset(rotation_offset) {}
};
enum class Color { White,
                   Yellow,
                   Red,
                   Pink,
                   Blue,
                   Green };
enum class RelativePosition { Upside,
                              Downside,
                              Leftside,
                              Rightside };
enum class Direction { Clockwise,
                       CounterClockwise };
struct CubeAction {
    bool is_middle_layer;
    Surface surface;
    Direction direction;
};

struct CubeActionStandard {
    int standard_surface_index;
    bool is_positive_direction;
};

constexpr CubeActionStandard StandardActions[] = {
    {0, true},
    {1, true},
    {2, true},
    {3, true},
    {4, true},
    {5, true},
    {6, true},
    {7, true},
    {8, true},
    {0, false},
    {1, false},
    {2, false},
    {3, false},
    {4, false},
    {5, false},
    {6, false},
    {7, false},
    {8, false},
};

inline Surface ToSurface(const std::string& surface_name) {
    Surface surface;
    switch (surface_name[0]) {
        case 'u':
            surface = Surface::Up;
            break;
        case 'd':
            surface = Surface::Down;
            break;
        case 'f':
            surface = Surface::Front;
            break;
        case 'b':
            surface = Surface::Back;
            break;
        case 'l':
            surface = Surface::Left;
            break;
        case 'r':
            surface = Surface::Right;
            break;

        default:
            throw std::invalid_argument("Invalid cube surface name: " + surface_name);
    }
    return surface;
}

inline std::string ToSurfaceName(Surface surface) {
    std::string surface_name;
    switch (surface) {
        case Surface::Up:
            surface_name = "up";
            break;
        case Surface::Down:
            surface_name = "down";
            break;
        case Surface::Front:
            surface_name = "front";
            break;
        case Surface::Back:
            surface_name = "back";
            break;
        case Surface::Left:
            surface_name = "left";
            break;
        case Surface::Right:
            surface_name = "right";
            break;
    }
    return surface_name;
}

inline Color ToColor(const std::string& color_name) {
    Color color;
    switch (color_name[0]) {
        case 'w':
            color = Color::White;
            break;
        case 'y':
            color = Color::Yellow;
            break;
        case 'r':
            color = Color::Red;
            break;
        case 'p':
            color = Color::Pink;
            break;
        case 'b':
            color = Color::Blue;
            break;
        case 'g':
            color = Color::Green;
            break;

        default:
            throw std::invalid_argument("Invalid cube color name: " + color_name);
    }
    return color;
}

inline char ToColorName(Color color) {
    char color_name;
    switch (color) {
        case Color::White:
            color_name = 'w';
            break;
        case Color::Yellow:
            color_name = 'y';
            break;
        case Color::Red:
            color_name = 'r';
            break;
        case Color::Pink:
            color_name = 'p';
            break;
        case Color::Blue:
            color_name = 'b';
            break;
        case Color::Green:
            color_name = 'g';
            break;
        default:
            throw std::runtime_error("Invalid color name");
    }
    return color_name;
}

inline RotationPosition operator++(RotationPosition& rotation_position) {
    rotation_position = static_cast<RotationPosition>((rotation_position + 1) % RoundCount);
    return rotation_position;
}

inline RotationPosition operator+(RotationPosition rotation_position, int offset) {
    if (offset < 0)
        offset = offset + (1 + (-offset) / RoundCount) * RoundCount;
    return static_cast<RotationPosition>(((int)rotation_position + offset * 2) % RoundCount);
}

inline RotationPosition operator-(RotationPosition rotation_position, int offset) {
    return rotation_position + (-offset);
}

inline RelativePosition operator+(RelativePosition relative_position, Direction direction) {
    switch (relative_position) {
        case RelativePosition::Upside:
            return direction == Direction::Clockwise ? RelativePosition::Rightside : RelativePosition::Leftside;
        case RelativePosition::Downside:
            return direction == Direction::Clockwise ? RelativePosition::Leftside : RelativePosition::Rightside;
        case RelativePosition::Leftside:
            return direction == Direction::Clockwise ? RelativePosition::Upside : RelativePosition::Downside;
        case RelativePosition::Rightside:
            return direction == Direction::Clockwise ? RelativePosition::Downside : RelativePosition::Upside;
        default:
            throw std::invalid_argument("Invalid relative position");
    }
}
inline SurfaceWithRotationOffset operator+(Surface surface, RelativePosition relative_position) {
    switch (surface) {
        case Surface::Front:
            switch (relative_position) {
                case RelativePosition::Upside:
                    return SurfaceWithRotationOffset(Surface::Up, 0);
                case RelativePosition::Downside:
                    return SurfaceWithRotationOffset(Surface::Down, 0);
                case RelativePosition::Leftside:
                    return SurfaceWithRotationOffset(Surface::Left, -1);
                case RelativePosition::Rightside:
                    return SurfaceWithRotationOffset(Surface::Right, +1);
                default:
                    throw std::runtime_error("Invalid surface");
            }
        case Surface::Back:
            switch (relative_position) {
                case RelativePosition::Upside:
                    return SurfaceWithRotationOffset(Surface::Down, 0);
                case RelativePosition::Downside:
                    return SurfaceWithRotationOffset(Surface::Up, 0);
                case RelativePosition::Leftside:
                    return SurfaceWithRotationOffset(Surface::Left, +1);
                case RelativePosition::Rightside:
                    return SurfaceWithRotationOffset(Surface::Right, -1);
                default:
                    throw std::runtime_error("Invalid surface");
            }
        case Surface::Left:
            switch (relative_position) {
                case RelativePosition::Upside:
                    return SurfaceWithRotationOffset(Surface::Back, -1);
                case RelativePosition::Downside:
                    return SurfaceWithRotationOffset(Surface::Front, +1);
                case RelativePosition::Leftside:
                    return SurfaceWithRotationOffset(Surface::Down, +2);
                case RelativePosition::Rightside:
                    return SurfaceWithRotationOffset(Surface::Up, 0);
                default:
                    throw std::runtime_error("Invalid surface");
            }
        case Surface::Right:
            switch (relative_position) {
                case RelativePosition::Upside:
                    return SurfaceWithRotationOffset(Surface::Back, +1);
                case RelativePosition::Downside:
                    return SurfaceWithRotationOffset(Surface::Front, -1);
                case RelativePosition::Leftside:
                    return SurfaceWithRotationOffset(Surface::Up, 0);
                case RelativePosition::Rightside:
                    return SurfaceWithRotationOffset(Surface::Down, -2);
                default:
                    throw std::runtime_error("Invalid surface");
            }
        case Surface::Up:
            switch (relative_position) {
                case RelativePosition::Upside:
                    return SurfaceWithRotationOffset(Surface::Back, 0);
                case RelativePosition::Downside:
                    return SurfaceWithRotationOffset(Surface::Front, 0);
                case RelativePosition::Leftside:
                    return SurfaceWithRotationOffset(Surface::Left, 0);
                case RelativePosition::Rightside:
                    return SurfaceWithRotationOffset(Surface::Right, 0);
                default:
                    throw std::runtime_error("Invalid surface");
            }
        case Surface::Down:
            switch (relative_position) {
                case RelativePosition::Upside:
                    return SurfaceWithRotationOffset(Surface::Front, 0);
                case RelativePosition::Downside:
                    return SurfaceWithRotationOffset(Surface::Back, 0);
                case RelativePosition::Leftside:
                    return SurfaceWithRotationOffset(Surface::Left, -2);
                case RelativePosition::Rightside:
                    return SurfaceWithRotationOffset(Surface::Right, +2);
                default:
                    throw std::runtime_error("Invalid surface");
            }
        default:
            throw std::invalid_argument("Invalid surface");
    }
}

inline CubeActionStandard ConvertCubeActionFormat(CubeAction action) {
    CubeActionStandard standard_action;
    if (action.is_middle_layer) {
        switch (action.surface) {
            case Surface::Left:
                standard_action.standard_surface_index = 1;
                standard_action.is_positive_direction = action.direction == Direction::Clockwise;
                break;
            case Surface::Right:
                standard_action.standard_surface_index = 1;
                standard_action.is_positive_direction = action.direction == Direction::CounterClockwise;
                break;
            case Surface::Up:
                standard_action.standard_surface_index = 4;
                standard_action.is_positive_direction = action.direction == Direction::Clockwise;
                break;
            case Surface::Down:
                standard_action.standard_surface_index = 4;
                standard_action.is_positive_direction = action.direction == Direction::CounterClockwise;
                break;
            case Surface::Front:
                standard_action.standard_surface_index = 7;
                standard_action.is_positive_direction = action.direction == Direction::Clockwise;
                break;
            case Surface::Back:
                standard_action.standard_surface_index = 7;
                standard_action.is_positive_direction = action.direction == Direction::CounterClockwise;
                break;
            default:
                throw std::runtime_error("Invalid surface");
        }
    } else {
        switch (action.surface) {
            case Surface::Left:
                standard_action.standard_surface_index = 0;
                standard_action.is_positive_direction = action.direction == Direction::Clockwise;
                break;
            case Surface::Right:
                standard_action.standard_surface_index = 2;
                standard_action.is_positive_direction = action.direction == Direction::CounterClockwise;
                break;
            case Surface::Up:
                standard_action.standard_surface_index = 5;
                standard_action.is_positive_direction = action.direction == Direction::Clockwise;
                break;
            case Surface::Down:
                standard_action.standard_surface_index = 3;
                standard_action.is_positive_direction = action.direction == Direction::CounterClockwise;
                break;
            case Surface::Front:
                standard_action.standard_surface_index = 6;
                standard_action.is_positive_direction = action.direction == Direction::Clockwise;
                break;
            case Surface::Back:
                standard_action.standard_surface_index = 8;
                standard_action.is_positive_direction = action.direction == Direction::CounterClockwise;
                break;
            default:
                throw std::runtime_error("Invalid surface");
        }
    }
    return standard_action;
}

inline bool operator==(const CubeActionStandard& a, const CubeActionStandard& b) {
    return a.standard_surface_index == b.standard_surface_index && a.is_positive_direction == b.is_positive_direction;
}

inline bool operator!=(const CubeActionStandard& a, const CubeActionStandard& b) {
    return !(a == b);
}

inline CubeAction ConvertCubeActionFormat(CubeActionStandard standard_action) {
    CubeAction action;
    if (standard_action.standard_surface_index == 1 || standard_action.standard_surface_index == 4 || standard_action.standard_surface_index == 7) {
        action.is_middle_layer = true;
    } else {
        action.is_middle_layer = false;
    }
    switch (standard_action.standard_surface_index) {
        case 0:
            action.surface = Surface::Left;
            action.direction = standard_action.is_positive_direction ? Direction::Clockwise : Direction::CounterClockwise;
            break;
        case 1:
            action.surface = Surface::Left;
            action.direction = standard_action.is_positive_direction ? Direction::Clockwise : Direction::CounterClockwise;
            break;
        case 2:
            action.surface = Surface::Right;
            action.direction = standard_action.is_positive_direction ? Direction::CounterClockwise : Direction::Clockwise;
            break;
        case 3:
            action.surface = Surface::Down;
            action.direction = standard_action.is_positive_direction ? Direction::CounterClockwise : Direction::Clockwise;
            break;
        case 4:
            action.surface = Surface::Up;
            action.direction = standard_action.is_positive_direction ? Direction::Clockwise : Direction::CounterClockwise;
            break;
        case 5:
            action.surface = Surface::Up;
            action.direction = standard_action.is_positive_direction ? Direction::Clockwise : Direction::CounterClockwise;
            break;
        case 6:
            action.surface = Surface::Front;
            action.direction = standard_action.is_positive_direction ? Direction::Clockwise : Direction::CounterClockwise;
            break;
        case 7:
            action.surface = Surface::Front;
            action.direction = standard_action.is_positive_direction ? Direction::Clockwise : Direction::CounterClockwise;
            break;
        case 8:
            action.surface = Surface::Back;
            action.direction = standard_action.is_positive_direction ? Direction::CounterClockwise : Direction::Clockwise;
            break;
    }
    return action;
}