//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MAP_H_
#define HEXAGON_MAP_H_

#include <vector>
#include "tile.hpp"
#include "unit.hpp"

namespace hexagon::model
{
    class map
    {
       public:
        using tile_container = std::vector<tile>;

       private:
        tile_container tiles_;
        int width_ = 0;

       public:
        map(tile_container tiles, int width);
        map() = default;

        tile_container::iterator find(int column, int row);
        tile_container::const_iterator find(int column, int row) const;

        auto begin() const { return tiles_.begin(); }
        auto begin() { return tiles_.begin(); }
        auto end() const { return tiles_.end(); }
        auto end() { return tiles_.end(); }

        tile_container::size_type size() const { return tiles_.size(); }

        int width() const { return width_; }
        int height() const { return tiles_.size() / width_; }

        tile_container::iterator find_unit(const unit&);

        tile_container::iterator spawn(unit&);
    };
}  // namespace hexagon

#endif
