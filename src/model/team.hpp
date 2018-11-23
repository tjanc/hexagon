//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNIT_H_
#define HEXAGON_MODEL_UNIT_H_

#include <vector>

#include "unit.hpp"

namespace hexagon::model
{
    struct team {
        using unit_container = std::vector<unit>;
        unit_container units;
        team() : units{unit{}} {}
    };
}  // namespace hexagon::model

#endif
