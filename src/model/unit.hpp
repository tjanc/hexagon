//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_UNIT_H_
#define HEXAGON_UNIT_H_

namespace hexagon
{
    enum class race { human };

    using level = int;
    using range = int;

    struct unit {
        race race_ = race::human;
        level level_ = 1;
        range range_ = 3;
        unit() = default;
    };
}  // namespace hexagon

#endif
