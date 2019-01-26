//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "world_facet.hpp"

#include <hexagon/state/world_state.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

world_facet::world_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height}
{
}

void world_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
}

int world_facet::width() const noexcept { return dimensions_.w; }

int world_facet::height() const noexcept { return dimensions_.h; }

void hexagon::client::draw(graphics&, const world_facet&,
                           const state::world_state&)
{
    //
}
