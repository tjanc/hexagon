//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/joined_battle_message.hpp>

#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/std_io.hpp>
#include <hexagon/protocol/io/team_io.hpp>
#include <hexagon/protocol/io/unit_io.hpp>

namespace hexagon::protocol
{
    joined_battle_message::joined_battle_message(model::team t,
                                                 placement_container p) noexcept
        : team{std::move(t)}, placements{std::move(p)}
    {
    }

    std::istream& operator>>(std::istream& in, joined_battle_message& msg)
    {
        using namespace hexagon::protocol::io;

        in >> msg.team;
        in >> msg.placements;

        return in;
    }

    std::ostream& operator<<(std::ostream& out,
                             const joined_battle_message& msg)
    {
        using namespace hexagon::protocol::io;

        out << msg.team << ' ' << msg.placements;

        return out;
    }
}  // namespace hexagon::protocol
