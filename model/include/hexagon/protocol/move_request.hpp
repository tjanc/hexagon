//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MOVE_REQUEST_H_
#define HEXAGON_PROTOCOL_MOVE_REQUEST_H_

#include <iostream>
#include <utility>
#include <vector>

#include <hexagon/model/map.hpp>

namespace hexagon::protocol
{
    static constexpr const char* MOVE_REQUEST_ID = "MOVEQ";

    struct move_request {
        model::basic_map_index source;
        model::basic_map_index target;

        move_request() = default;
        move_request(model::basic_map_index source,
                              model::basic_map_index target)
            : source{source}, target{target}
        {
        }
    };

    std::istream& operator>>(std::istream& in, move_request& msg);
    std::ostream& operator<<(std::ostream& out, const move_request& msg);
}  // namespace hexagon::protocol

#endif
