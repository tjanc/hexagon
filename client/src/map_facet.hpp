//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MAP_FACET_H_
#define HEXAGON_CLIENT_MAP_FACET_H_

#include <hexagon/model/map.hpp>

#include <tuple>

namespace hexagon::client
{
    class canvas;
    class moving_controller;
}  // namespace hexagon::client

namespace hexagon::client
{
    class map_facet
    {
        // start point
        int x_ = 0;
        int y_ = 0;

        model::map::tile_container::iterator hover_tile_;

       public:
        explicit map_facet(int x, int y) noexcept;

       public:
        std::pair<int, int> transpose(int x, int y) const noexcept;

       public:
        void hover(model::map::tile_container::iterator) noexcept;
        model::map::tile_container::iterator hover() const noexcept;

       public:
        void draw(canvas&, const model::map&,
                  model::map::tile_container::const_iterator) const;
    };
}  // namespace hexagon::client

#endif
