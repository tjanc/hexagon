//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MOVE_MESSAGE_H_
#define HEXAGON_PROTOCOL_MOVE_MESSAGE_H_

#include <iostream>
#include <utility>
#include <vector>

#include <hexagon/model/map.hpp>

namespace hexagon::protocol
{
    static constexpr const char* MOVE_MESSAGE_ID = "MOVE";

    struct move_message {
        model::basic_map_index source;
        model::basic_map_index target;

        move_message() = default;
        move_message(model::basic_map_index source,
                     model::basic_map_index target)
            : source{source}, target{target}
        {
        }
    };

    std::istream& operator>>(std::istream& in, move_message& msg);
    std::ostream& operator<<(std::ostream& out, const move_message& msg);
}  // namespace hexagon::protocol

#endif
