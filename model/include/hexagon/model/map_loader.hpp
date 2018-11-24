//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_MAP_LOADER_H_
#define HEXAGON_MODEL_MAP_LOADER_H_

#include "map.hpp"
#include "unit.hpp"

namespace hexagon
{
    struct file_not_found {
        const char* file;
    };

    struct map_syntax_error {
    };

    map load_map(const char* file_name);

}  // namespace hexagon

#endif
