//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_facet.hpp"

#include <hexagon/model/unit_moving.hpp>

#include "moving_controller.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

battle_facet::battle_facet(int width, int height) noexcept
    : width_{width}, height_{height}, map_facet_{0, 0}
{
}

void battle_facet::draw(canvas& c, const unit_moving& model) const
{
    map_facet_.draw(c, model);
}

const map_facet& battle_facet::map() const noexcept { return map_facet_; }
map_facet& battle_facet::map() noexcept { return map_facet_; }

int battle_facet::width() const noexcept { return width_; }
int battle_facet::height() const noexcept { return height_; }
