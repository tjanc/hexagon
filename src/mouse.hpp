//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MOUSE_H_
#define HEXAGON_MOUSE_H_

#include <cstdint>

namespace hexagon
{
    struct mouse {
        bool toggled = false;
        mouse() = default;
    };
}  // namespace hexagon

#endif
