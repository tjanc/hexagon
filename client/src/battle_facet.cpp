//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_facet.hpp"

#include "moving_controller.hpp"

using namespace hexagon::client;

battle_facet::battle_facet(int width, int height) noexcept
    : width_{width}, height_{height}, map_facet_{0, 0}
{
}

void battle_facet::draw(canvas& c, const moving_controller& controller) const
{
    const auto& m = controller.model().get_map();
    map_facet_.draw(c, m, m.find_unit(*controller.unit()));
}
