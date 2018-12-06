//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/map.hpp>

#include <algorithm>
#include <iostream>

using namespace hexagon::model;
namespace
{
    template <typename It>
    It find_spawnable(It begin, It end)
    {
        return std::find_if(begin, end, [](const tile& t) {  //
            return t.is_spawnable();
        });
    }
}  // namespace

map::map(map::tile_container tiles, int width) : tiles_(tiles), width_(width) {}

map::tile_container::iterator map::find(int column, int row)
{
    if (column >= width_) return tiles_.end();

    const int offset = row * width_ + column;
    if (offset >= tiles_.size()) return tiles_.end();

    return std::next(tiles_.begin(), offset);
}

map::tile_container::const_iterator map::find(int column, int row) const
{
    if (column >= width_) return tiles_.end();

    const int offset = row * width_ + column;
    if (offset >= tiles_.size()) return tiles_.end();

    return std::next(tiles_.begin(), offset);
}

map::tile_container::iterator map::spawn(unit& u)
{
    using std::begin;
    using std::end;

    auto it = find_spawnable(begin(tiles_), end(tiles_));
    if (it == end(tiles_)) {
        std::cout << "no spawn point found\n";
        return it;
    }
    std::cout << "spawn point found\n";
    it->attach(u);
    return it;
}

map::tile_container::iterator map::find_unit(const unit& u) noexcept
{
    return std::find_if(     //
        std::begin(tiles_),  //
        std::end(tiles_),    //
        [&u](const auto& tile) { return tile.has_unit(u); });
}

map::tile_container::const_iterator map::find_unit(const unit& u) const noexcept
{
    return std::find_if(     //
        std::begin(tiles_),  //
        std::end(tiles_),    //
        [&u](const auto& tile) { return tile.has_unit(u); });
}
