#ifndef HEXAGON_MAP_FACET_H_
#define HEXAGON_MAP_FACET_H_

#include <SDL2/SDL.h>

#include "model/map.hpp"

#include "sdl/texture.hpp"

#include "map_textures.hpp"
#include "unit_facet.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon
{
    class map_facet
    {
        // textures
        map_textures tile_textures_;
        unit_texture_cache unit_textures_;

        // box
        int width_ = 0;
        int height_ = 0;

        map::tile_container::iterator hover_tile_;

       public:
        explicit map_facet(sdl::renderer&);

       public:
        void hover(map::tile_container::iterator t) { hover_tile_ = t; }
        map::tile_container::iterator hover() { return hover_tile_; }

        void mouse_over(int x, int y, map&);

        void draw(sdl::renderer&, const map&);
    };
}  // namespace hexagon

#endif
