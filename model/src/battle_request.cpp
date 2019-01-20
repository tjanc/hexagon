//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/battle_request.hpp>

#include <cassert>

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, battle_request& msg)
    {
        in >> msg.id;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const battle_request& msg)
    {
        out << msg.id;

        return out;
    }
}  // namespace hexagon::protocol
