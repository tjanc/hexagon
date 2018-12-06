//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "canvas.hpp"
#include "sdl/window.hpp"

using namespace hexagon::client;

canvas::canvas(sdl::window& w)
    : renderer_{w}, tile_textures_{renderer_}, unit_textures_{renderer_}
{
}
