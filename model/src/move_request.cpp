//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/move_request.hpp>

#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/path_io.hpp>

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, move_request& msg)
    {
        using namespace hexagon::protocol::io;
        in >> msg.route;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const move_request& msg)
    {
        using namespace hexagon::protocol::io;
        out << msg.route;

        return out;
    }
}  // namespace hexagon::protocol
