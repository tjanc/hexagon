//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/battle_message.hpp>

#include <hexagon/model/battle.hpp>
#include <hexagon/protocol/io/battle_io.hpp>
#include <iostream>

namespace hexagon::protocol
{
    battle_message::battle_message(model::battle b, std::size_t tid) noexcept
        : battle{std::move(b)}, team_id{tid}
    {
    }

    std::istream& operator>>(std::istream& in, battle_message& msg)
    {
        using namespace hexagon::protocol::io;

        in >> msg.team_id;
        in >> msg.battle;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const battle_message& msg)
    {
        using namespace hexagon::protocol::io;

        out << msg.team_id << ' ' << msg.battle;

        return out;
    }
}  // namespace hexagon::protocol
