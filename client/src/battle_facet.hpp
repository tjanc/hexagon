//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_FACET_H_
#define HEXAGON_CLIENT_BATTLE_FACET_H_

#include "battle_controller.hpp"
#include "map_facet.hpp"
#include "sdl/texture.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon::client
{
    class battle_facet
    {
        map_facet map_facet_;

        // sdl::texture active_unit_name_;

        int width_ = 0;
        int height_ = 0;

       public:
        explicit battle_facet(sdl::renderer&);

       public:
        void mouse_move(battle_controller& model);
        void mouse_down(battle_controller& model);
        void mouse_up(battle_controller& model);

        void draw(sdl::renderer& r, battle_controller& model);
    };
}  // namespace hexagon::client

#endif
