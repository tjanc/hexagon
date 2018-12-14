//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "world_facet.hpp"

#include <hexagon/model/world.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

world_facet::world_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height}
{
}

void world_facet::draw(canvas& c, const world& model) const {}
