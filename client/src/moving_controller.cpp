//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "moving_controller.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>
#include <hexagon/protocol/io/unit_io.hpp>

#include "battle_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

moving_controller::moving_controller(battle_facet facet,
                                     model::unit_moving model) noexcept
    : model_{std::move(model)}, facet_{std::move(facet)}, commands_{}
{
    std::cout << "Move phase.\n";
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
            model_.move(target);

            if (!model_.has_next()) {
                for (auto&& cmd : commands_) {
                    std::cout << "TODO: commit movement\n";
                }
                c.state(moved_controller{std::move(*this)});
            } else {
                model_.next();
            }
        }
    }
}

void moving_controller::draw(canvas& c) const { facet_.draw(c, model_); }

const unit_moving& moving_controller::model() const noexcept { return model_; }

unit_moving& moving_controller::model() noexcept { return model_; }

const battle_facet& moving_controller::facet() const noexcept { return facet_; }

battle_facet& moving_controller::facet() noexcept { return facet_; }
