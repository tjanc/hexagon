//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "unit_facet.hpp"

#include "sdl/surface.hpp"
#include "sdl/texture.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::sdl;

unit_texture_cache::unit_texture_cache(renderer& renderer)
    : units_{load_texture(renderer, "assets/unit_warrior.png"),
             load_texture(renderer, "assets/unit_mage.png")}
{
}

texture& unit_texture_cache::at(unit_job r, perspective p)
{
    switch (r) {
        default:
        case unit_job::warrior:
            return units_.at(0);
        case unit_job::mage:
            return units_.at(1);
    }
}
