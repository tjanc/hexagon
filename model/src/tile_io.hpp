//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_TILE_IO_H_
#define HEXAGON_PROTOCOL_TILE_IO_H_

#include <hexagon/model/tile.hpp>
#include <iostream>

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream& in, model::tile_type& t);
    std::ostream& operator<<(std::ostream& out, const model::tile_type& t);

    std::istream& operator>>(std::istream& in, model::tile& msg);
    std::ostream& operator<<(std::ostream& out, const model::tile& msg);
}  // namespace hexagon::protocol::io

#endif
