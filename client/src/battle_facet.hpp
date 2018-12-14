//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_FACET_H_
#define HEXAGON_CLIENT_BATTLE_FACET_H_

#include <SDL.h>
#include "map_facet.hpp"

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
    class battle_facet
    {
        SDL_Rect dimensions_;
        map_facet map_facet_;

       public:
        battle_facet(int x, int y, int width, int height) noexcept;

       public:
        void draw(canvas&, const model::unit_moving&) const;
        void draw(canvas&, const model::unit_moved&) const;

       public:
        const map_facet& map() const noexcept;
        map_facet& map() noexcept;

       public:
        int width() const noexcept;
        int height() const noexcept;
    };
}  // namespace hexagon::client

#endif
