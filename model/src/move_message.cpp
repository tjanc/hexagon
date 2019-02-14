//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/move_message.hpp>

#include <hexagon/protocol/io/path_io.hpp>

namespace hexagon::protocol
{
    move_message::move_message(hexagon::model::vertex_path p,
                               std::uint16_t c) noexcept
        : route{std::move(p)}, cost{c}
    {
    }

    std::istream& operator>>(std::istream& in, move_message& msg)
    {
        using namespace hexagon::protocol::io;
        in >> msg.route;
        in >> msg.cost;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const move_message& msg)
    {
        using namespace hexagon::protocol::io;
        out << msg.route << ' ' << msg.cost;

        return out;
    }
}  // namespace hexagon::protocol
