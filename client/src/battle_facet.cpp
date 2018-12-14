//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_facet.hpp"

#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

battle_facet::battle_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height},
      map_facet_{x, y, width, height}
{
}

void battle_facet::draw(canvas& c, const unit_moving& model) const
{
    map_facet_.draw(c, model);
}

void battle_facet::draw(canvas& c, const units_moved& model) const
{
    map_facet_.draw(c, model);
}

const map_facet& battle_facet::map() const noexcept { return map_facet_; }
map_facet& battle_facet::map() noexcept { return map_facet_; }

int battle_facet::width() const noexcept { return dimensions_.w; }
int battle_facet::height() const noexcept { return dimensions_.h; }
