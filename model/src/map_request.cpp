//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/map_request.hpp>

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, map_request& msg)
    {
        in >> msg.id;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const map_request& msg)
    {
        out << MAP_REQUEST_ID << ' ';
        out << msg.id << ' ';

        return out;
    }
}  // namespace hexagon::protocol
