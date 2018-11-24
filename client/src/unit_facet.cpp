//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "unit_facet.hpp"

#include "sdl/surface.hpp"
#include "sdl/texture.hpp"

using namespace hexagon;
using namespace hexagon::model;

unit_texture_cache::unit_texture_cache(sdl::renderer& renderer)
    : units_{sdl::load_texture(renderer, "assets/unit_warrior.png")}
{
}

sdl::texture& unit_texture_cache::at(race r, perspective p)
{
    switch (r) {
        default:
        case race::human:
            return units_.at(0);
    }
}
