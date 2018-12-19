//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/map_io.hpp>

#include <hexagon/protocol/io/tile_io.hpp>
#include <iostream>

using namespace hexagon::model;
using namespace hexagon::protocol;

std::istream& io::operator>>(std::istream& in, basic_map_index& obj)
{
    in >> obj.x;
    in >> obj.y;

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const basic_map_index& obj)
{
    out << obj.x <<    //
        ' ' << obj.y;  //

    return out;
}
