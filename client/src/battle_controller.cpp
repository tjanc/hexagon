//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_controller.hpp"

#include <iostream>

using namespace hexagon::client;
using namespace hexagon::model;

battle_controller::battle_controller(battle b, std::size_t team_index) noexcept
    : state_{moving_controller{unit_moving{std::move(b), team_index}, 0, 0}}
{
    std::cout << "Battle loaded, controlling team " << team_index << ".\n";
}

void battle_controller::update(const mouse& m) noexcept
{
    std::visit(
        [&m, &self = *this](auto& s) {  //
            s.update(self, m);
        },
        state_);
}

void battle_controller::draw(canvas& c) const
{
    std::visit(
        [&c](auto& s) {  //
            s.draw(c);
        },
        state_);
}
