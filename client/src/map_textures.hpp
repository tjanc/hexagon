//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MAP_TEXTURES_H_
#define HEXAGON_CLIENT_MAP_TEXTURES_H_

#include <array>

#include <hexagon/model/tile.hpp>
#include "sdl/texture.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon::client
{
    class map_textures
    {
        std::array<sdl::texture, 9> tiles_;

       public:
        explicit map_textures(sdl::renderer& renderer);

        sdl::texture& tile_surface(model::tile_type type);
        sdl::texture& tile_filler(model::tile_type type);
        sdl::texture& tile_hover();

        auto begin() const { return tiles_.begin(); }
        auto end() const { return tiles_.end(); }
    };
}  // namespace hexagon

#endif
