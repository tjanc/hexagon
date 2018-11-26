//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MAP_FACET_H_
#define HEXAGON_CLIENT_MAP_FACET_H_

#include <hexagon/model/map.hpp>

#include "sdl/texture.hpp"

#include "map_textures.hpp"
#include "unit_facet.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon::client
{
    class map_facet
    {
        // textures
        map_textures tile_textures_;
        unit_texture_cache unit_textures_;

        // box
        int width_ = 0;
        int height_ = 0;

        model::map::tile_container::iterator hover_tile_;

       public:
        explicit map_facet(sdl::renderer&);

       public:
        void hover(model::map::tile_container::iterator t) { hover_tile_ = t; }
        model::map::tile_container::iterator hover() { return hover_tile_; }

        void mouse_over(int x, int y, model::map&);

        void draw(sdl::renderer&, const model::map&);
    };
}  // namespace hexagon

#endif
