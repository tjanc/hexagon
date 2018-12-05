//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MAP_H_
#define HEXAGON_MAP_H_

#include <vector>
#include "tile.hpp"
#include "unit.hpp"

namespace hexagon::model
{
    using map_id = std::size_t;

    class map
    {
       public:
        using tile_container = std::vector<tile>;
        using iterator = tile_container::iterator;
        using const_iterator = tile_container::const_iterator;

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

        tile_container::iterator find_unit(const unit&) noexcept;
        tile_container::const_iterator find_unit(const unit&) const noexcept;

        tile_container::iterator spawn(unit&);
    };

    template <typename Visitor>
    void tiles(const map& m, Visitor v)
    {
        for (int y = 0; y < m.height(); ++y)
            for (int x = 0; x < m.width(); ++x) {
                auto it = m.find(x, y);
                v(it, x, y);
            }
    }

    template <std::size_t Radius, typename Visitor>
    void tiles_around(const map& m, map::const_iterator c, Visitor v)
    {
        const auto width = m.width();
        const auto b = m.begin();
        const auto e = m.end();
        const auto c_offset = c - b;
        const auto c_x_offset = c_offset / width - Radius;
        const auto c_y_offset = c_offset % width - Radius;

        for (int y = 0; y < Radius * 2 + 1; ++y)
            for (int x = 0; x < Radius * 2 + 1; ++x) {
                auto it = m.find(x + c_x_offset, y + c_y_offset);
                if (it != m.end()) v(it, x, y);
            }
    }
}  // namespace hexagon::model

#endif
