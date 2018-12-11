//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "moving_controller.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>

#include "battle_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

moving_controller::moving_controller(model::unit_moving model,  //
                                     int width, int height) noexcept
    : model_{std::move(model)}, facet_{width, height}, commands_{}
{
}

void moving_controller::update(battle_controller& c, const mouse& m) noexcept
{
    map& field = model_.battlefield().get_map();

    if (m.moved()) {
        auto idx = facet_.map().transpose(m.x, m.y);
        if (contains(field, idx)) facet_.map().hover(idx);
    }

    if (m.released()) {
        auto source = model_.position();
        auto target = facet_.map().hover();

        assert(contains(field, source));
        if (model_.reachable(target)) {
            commands_.emplace_back(source, target);

            if (!model_.has_next()) {
                std::cout << "TODO: commit movements\n";
            }

            c.state(moving_controller{move_unit(std::move(model_), target),
                                      facet_.width(), facet_.height()});
        }
    }
}

void moving_controller::draw(canvas& c) const { facet_.draw(c, model_); }

const unit_moving& moving_controller::model() const noexcept { return model_; }

unit_moving& moving_controller::model() noexcept { return model_; }
