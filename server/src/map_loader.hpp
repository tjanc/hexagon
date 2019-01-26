//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_MAP_LOADER_H_
#define HEXAGON_SERVER_MAP_LOADER_H_

#include <hexagon/model/map.hpp>
#include <string>

namespace hexagon::server
{
    struct file_not_found {
        std::string file;
    };

    struct map_syntax_error {
    };

    model::map load_map(std::string file_name);
}  // namespace hexagon::server

#endif
