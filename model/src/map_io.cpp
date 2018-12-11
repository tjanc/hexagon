//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_io.hpp"

#include <iomanip>
#include <iostream>
#include "tile_io.hpp"

using namespace hexagon::model;
using namespace hexagon::protocol;

std::ostream& io::operator<<(std::ostream& out, const map& obj)
{
    using namespace hexagon::protocol::io;

    out << obj.width() << ' ';

    for (const auto& tile : obj) out << tile;

    return out;
}

std::istream& io::operator>>(std::istream& in, map& obj)
{
    using namespace hexagon::protocol::io;

    std::uint32_t width = 0;
    in >> width;

    map::tiles_container tiles;
    tiles.reserve(width);

    tile t;
    while (in >> t) {
        tile n;
        std::swap(t, n);
        tiles.emplace_back(std::move(n));
    }

    obj = map{std::move(tiles), width};

    return in;
}
