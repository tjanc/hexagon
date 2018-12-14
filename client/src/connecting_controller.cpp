//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "connecting_controller.hpp"

#include "battle_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

connecting_controller::connecting_controller(connecting_facet facet) noexcept
    : model_{}, facet_{std::move(facet)}
{
}

void connecting_controller::update(game_controller& c, const mouse& m) noexcept
{
}

void connecting_controller::draw(canvas& c) const { facet_.draw(c, model_); }
