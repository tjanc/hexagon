//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_UNIT_IO_H_
#define HEXAGON_PROTOCOL_UNIT_IO_H_

#include <hexagon/model/unit.hpp>
#include <iostream>

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream& in, model::unit& t);
    std::ostream& operator<<(std::ostream& out, const model::unit& t);
}  // namespace hexagon::protocol::io

#endif
