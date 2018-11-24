#include "map_textures.hpp"

#include <SDL.h>

#include <hexagon/model/map.hpp>
#include <hexagon/model/map_loader.hpp>

#include "sdl/renderer.hpp"

using namespace hexagon;

map_textures::map_textures(sdl::renderer& renderer)
    : tiles_{sdl::load_texture(renderer, "assets/grs00.png"),
             sdl::load_texture(renderer, "assets/grs01.png"),
             sdl::load_texture(renderer, "assets/grs02.png"),
             sdl::load_texture(renderer, "assets/grs03.png"),
             sdl::load_texture(renderer, "assets/grs04.png"),
             sdl::load_texture(renderer, "assets/dirt00.png"),
             sdl::load_texture(renderer, "assets/dirt01.png"),
             sdl::load_texture(renderer, "assets/filler.png")}
{
}

sdl::texture& map_textures::tile_surface(tile_type type)
{
    switch (type) {
        default:
        case tile_type::grass0:
            return tiles_.at(0);
        case tile_type::grass1:
            return tiles_.at(1);
        case tile_type::grass2:
            return tiles_.at(2);
        case tile_type::grass3:
            return tiles_.at(3);
        case tile_type::grass4:
            return tiles_.at(4);
        case tile_type::dirt0:
            return tiles_.at(5);
        case tile_type::dirt1:
            return tiles_.at(6);
    }
}

sdl::texture& map_textures::tile_filler(tile_type type) { return tiles_.at(7); }
