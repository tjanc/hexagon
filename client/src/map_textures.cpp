//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_textures.hpp"

#include <SDL.h>

#include <hexagon/model/map.hpp>
#include <hexagon/model/map_loader.hpp>

#include "sdl/renderer.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::sdl;

map_textures::map_textures(renderer& renderer)
    : tiles_{load_texture(renderer, "assets/grs00_small.png"),
             load_texture(renderer, "assets/grs01_small.png"),
             load_texture(renderer, "assets/grs02_small.png"),
             load_texture(renderer, "assets/grs03_small.png"),
             load_texture(renderer, "assets/grs04_small.png"),
             load_texture(renderer, "assets/dirt00.png"),
             load_texture(renderer, "assets/dirt01.png"),
             load_texture(renderer, "assets/filler.png")}
{
}

texture& map_textures::tile_surface(tile_type type)
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

texture& map_textures::tile_filler(tile_type type) { return tiles_.at(7); }
