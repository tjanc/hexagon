//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MAP_IO_H_
#define HEXAGON_PROTOCOL_MAP_IO_H_

#include <hexagon/model/map.hpp>
#include <iostream>

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream& in, model::map& msg);
    std::ostream& operator<<(std::ostream& out, const model::map& msg);
}  // namespace hexagon::protocol::io

#endif
