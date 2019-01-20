//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/world_message.hpp>

#include <hexagon/protocol/io/team_io.hpp>

namespace hexagon::protocol
{
    world_message::world_message(model::team t) noexcept : team{std::move(t)} {}

    std::istream& operator>>(std::istream& in, world_message& msg)
    {
        using namespace hexagon::protocol::io;
        in >> msg.team;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const world_message& msg)
    {
        using namespace hexagon::protocol::io;
        out << msg.team;
        return out;
    }
}  // namespace hexagon::protocol
