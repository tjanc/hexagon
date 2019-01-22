//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MAP_FACET_H_
#define HEXAGON_CLIENT_MAP_FACET_H_

#include <SDL.h>
#include <hexagon/model/map.hpp>

namespace hexagon::model
{
    class unit_moving;
    class units_moved;
}  // namespace hexagon::model

namespace hexagon::state
{
    class battling_state;
}  // namespace hexagon::state

namespace hexagon::client
{
    class graphics;
    class moving_controller;
}  // namespace hexagon::client

namespace hexagon::client
{
    class map_facet
    {
        SDL_Rect dimensions_;
        model::basic_map_index hover_tile_;
        int x_offset_ = 0;
        int y_offset_ = 0;

       public:
        explicit map_facet(int x, int y, int width, int height) noexcept;

       public:
        void hover(model::basic_map_index) noexcept;
        model::basic_map_index hover() const noexcept;

       public:
        void resize(int w, int h) noexcept;
        const SDL_Rect& dimensions() const noexcept;
    };

    void draw(graphics&, const map_facet&, const state::battling_state&);
    model::basic_map_index transpose(const map_facet&,
                                     const state::battling_state&, int x,
                                     int y) noexcept;
}  // namespace hexagon::client

#endif
