//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_FACET_H_
#define HEXAGON_CLIENT_BATTLE_FACET_H_

#include "map_facet.hpp"

namespace hexagon::client
{
    class canvas;
    class moving_controller;
}

namespace hexagon::client
{
    class battle_facet
    {
        int width_ = 0;
        int height_ = 0;

        map_facet map_facet_;

       public:
        battle_facet(int width, int height) noexcept;

       public:
        void draw(canvas& r, const moving_controller& model) const;

       public:
        const map_facet& map() const noexcept { return map_facet_; }
        map_facet& map() noexcept { return map_facet_; }
    };
}  // namespace hexagon::client

#endif
