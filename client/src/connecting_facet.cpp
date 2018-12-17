//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "connecting_facet.hpp"

#include <hexagon/model/connecting.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

connecting_facet::connecting_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height}
{
}

void connecting_facet::draw(graphics& c, const connecting& model) const {}

void connecting_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
}

int connecting_facet::width() noexcept { return dimensions_.w; }

int connecting_facet::height() noexcept { return dimensions_.h; }
