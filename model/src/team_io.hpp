//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_TEAM_IO_H_
#define HEXAGON_PROTOCOL_TEAM_IO_H_

#include <hexagon/model/team.hpp>
#include <iostream>

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream& in, model::team& t);
    std::ostream& operator<<(std::ostream& out, const model::team& t);
}  // namespace hexagon::protocol::io

#endif
