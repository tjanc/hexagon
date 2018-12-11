//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MAP_FACET_H_
#define HEXAGON_CLIENT_MAP_FACET_H_

#include <hexagon/model/map.hpp>

#include <tuple>

namespace hexagon::model
{
    class unit_moving;
}

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

        model::basic_map_index hover_tile_;

       public:
        explicit map_facet(int x, int y) noexcept;

       public:
        model::basic_map_index transpose(int x, int y) const noexcept;

       public:
        void hover(model::basic_map_index) noexcept;
        model::basic_map_index hover() const noexcept;

       public:
        void draw(canvas&, const model::unit_moving&) const;
    };
}  // namespace hexagon::client

#endif
