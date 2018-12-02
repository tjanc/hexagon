//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MAP_RESPONSE_H_
#define HEXAGON_PROTOCOL_MAP_RESPONSE_H_

#include <iostream>

#include <hexagon/model/map.hpp>

namespace hexagon::protocol
{
    static constexpr const char* MAP_RESPONSE_ID = "MAP";

    struct map_response {
        hexagon::model::map map;
        map_response() = default;
    };

    std::istream& operator>>(std::istream& in, map_response& msg);
    std::ostream& operator<<(std::ostream& out, const map_response& msg);
}  // namespace hexagon::protocol

#endif
