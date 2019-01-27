//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/path_io.hpp>

#include <hexagon/model/path.hpp>
#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/std_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

std::istream& io::operator>>(std::istream& in, vertex_path& obj)
{
    in >> obj.vertices();
    return in;
}

std::ostream& io::operator<<(std::ostream& out, const vertex_path& obj)
{
    out << obj.vertices();
    return out;
}
