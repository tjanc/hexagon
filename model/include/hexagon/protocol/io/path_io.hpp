//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_PATH_IO_H_
#define HEXAGON_PROTOCOL_PATH_IO_H_

#include <iostream>

namespace hexagon::model
{
    class vertex_path;
}

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream&, model::vertex_path&);
    std::ostream& operator<<(std::ostream&, const model::vertex_path&);
}  // namespace hexagon::protocol::io

#endif
