#ifndef HEXAGON_MAP_TEXTURES_H_
#define HEXAGON_MAP_TEXTURES_H_

#include <array>

#include <hexagon/model/tile.hpp>
#include "sdl/texture.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon
{
    class map_textures
    {
        std::array<sdl::texture, 8> tiles_;

       public:
        explicit map_textures(sdl::renderer& renderer);

        sdl::texture& tile_surface(model::tile_type type);
        sdl::texture& tile_filler(model::tile_type type);

        auto begin() const { return tiles_.begin(); }
        auto end() const { return tiles_.end(); }
    };
}  // namespace hexagon

#endif
