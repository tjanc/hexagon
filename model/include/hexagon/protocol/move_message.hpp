//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MOVE_MESSAGE_H_
#define HEXAGON_PROTOCOL_MOVE_MESSAGE_H_

#include <hexagon/model/path.hpp>
#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* MOVE_MESSAGE_ID = "MOVE";

    struct move_message {
        model::vertex_path route;
        std::uint16_t cost = 0;

        move_message() = default;
        move_message(model::vertex_path, std::uint16_t cost) noexcept;
    };

    std::istream& operator>>(std::istream& in, move_message& msg);
    std::ostream& operator<<(std::ostream& out, const move_message& msg);
}  // namespace hexagon::protocol

#endif
