//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_BATTLE_IO_H_
#define HEXAGON_PROTOCOL_BATTLE_IO_H_

#include <iostream>

namespace hexagon::model
{
    class battle;
}

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream&, model::battle&);
    std::ostream& operator<<(std::ostream&, const model::battle&);
}  // namespace hexagon::protocol::io

#endif
