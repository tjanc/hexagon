//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_WORLD_MESSAGE_H_
#define HEXAGON_PROTOCOL_WORLD_MESSAGE_H_

#include <hexagon/model/team.hpp>
#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* WORLD_MESSAGE_ID = "WORLD";

    struct world_message {
        model::team team;

        world_message() = default;
        explicit world_message(model::team) noexcept;
    };

    std::istream& operator>>(std::istream& in, world_message& msg);
    std::ostream& operator<<(std::ostream& out, const world_message& msg);
}  // namespace hexagon::protocol

#endif
