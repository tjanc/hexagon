//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_FACET_H_
#define HEXAGON_CLIENT_BATTLE_FACET_H_

#include <SDL.h>
#include <hexagon/model/map.hpp>
#include "map_facet.hpp"

namespace hexagon::state
{
    class battling_state;
}

namespace hexagon::client
{
    class graphics;
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
        const map_facet& map() const noexcept;
        map_facet& map() noexcept;

       public:
        int width() const noexcept;
        int height() const noexcept;

        void resize(int w, int h) noexcept;
    };

    void draw(graphics&, const battle_facet&, const state::battling_state&);
    model::basic_map_index transpose(const battle_facet&,
                                     const state::battling_state&, int x,
                                     int y) noexcept;
}  // namespace hexagon::client

#endif
