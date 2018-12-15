//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/map_response.hpp>

#include <iomanip>
#include <iostream>
#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/tile_io.hpp>

using namespace hexagon::model;

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, map_response& msg)
    {
        using namespace hexagon::protocol::io;

        using hexagon::model::map;
        using hexagon::model::tile;

        in >> msg.map;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const map_response& msg)
    {
        using namespace hexagon::protocol::io;

        out << msg.map;

        return out;
    }
}  // namespace hexagon::protocol
