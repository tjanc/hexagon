//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_TEAM_IO_H_
#define HEXAGON_PROTOCOL_TEAM_IO_H_

#include <iostream>

namespace hexagon::model
{
    class team;
}

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream&, model::team&);
    std::ostream& operator<<(std::ostream&, const model::team&);
}  // namespace hexagon::protocol::io

#endif
