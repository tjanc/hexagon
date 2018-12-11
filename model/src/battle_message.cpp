//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/battle_message.hpp>

#include <iomanip>
#include <iostream>
#include "battle_io.hpp"

using namespace hexagon::model;

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, battle_message& msg)
    {
        using namespace hexagon::protocol::io;

        using hexagon::model::map;
        using hexagon::model::tile;

        in >> msg.map;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const battle_message& msg)
    {
        using namespace hexagon::protocol::io;

        out << BATTLE_MESSAGE_ID << ' ';
        out << msg.battle;

        return out;
    }
}  // namespace hexagon::protocol
