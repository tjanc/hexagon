//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "moved_controller.hpp"

#include <hexagon/model/map.hpp>
#include <iostream>

#include "battle_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

moved_controller::moved_controller(moving_controller moving) noexcept
    : model_{std::move(moving.model().battlefield()), moving.model().my_team()},
      facet_{std::move(moving.facet())}
{
    std::cout << "Move phase sync...\n";
}

void moved_controller::update(battle_controller& c, const mouse& m) noexcept
{
    map& field = model_.battlefield().get_map();

    if (m.moved()) {
        auto idx = facet_.map().transpose(m.x, m.y);
        if (contains(field, idx)) facet_.map().hover(idx);
    }
}

void moved_controller::draw(canvas& c) const { facet_.draw(c, model_); }

const unit_moved& moved_controller::model() const noexcept { return model_; }

unit_moved& moved_controller::model() noexcept { return model_; }
