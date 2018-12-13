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

moving_controller::moving_controller(model::unit_moving model,  //
                                     int width, int height) noexcept
    : model_{std::move(model)}, facet_{width, height}, commands_{}
{
    [](const auto& t) {
        std::cout << "Your team has " << t.units.size() << " units:\n";

        using namespace hexagon::protocol::io;
        for (const auto& u : t.units) std::cout << " - " << u << '\n';
    }(model_.my_team());
    std::cout << "Move phase loaded.\n";
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

            std::cout << "IT BLOWS\n";
            [](const auto& t) {
                std::cout << "Your team has " << t.units.size() << " units:\n";

                using namespace hexagon::protocol::io;
                for (const auto& u : t.units) std::cout << " - " << u << '\n';
            }(model_.my_team());

            move_unit(model_, target);
        }
    }
}

void moving_controller::draw(canvas& c) const { facet_.draw(c, model_); }

const unit_moving& moving_controller::model() const noexcept { return model_; }

unit_moving& moving_controller::model() noexcept { return model_; }
