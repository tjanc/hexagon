//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "connecting_facet.hpp"

#include <hexagon/state/connecting_state.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

connecting_facet::connecting_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height}
{
}

void connecting_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
}

int connecting_facet::width() const noexcept { return dimensions_.w; }

int connecting_facet::height() const noexcept { return dimensions_.h; }

void hexagon::client::draw(graphics&, const connecting_facet&,
                           const state::connecting_state&)
{
    //
}
