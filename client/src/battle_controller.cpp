//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_controller.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    auto start_battle(battle b, team t)
    {
        auto t_ptr = b.join(std::move(t));
        return moving_controller{unit_moving{std::move(b), t_ptr}, 0, 0};
    }
}  // namespace

battle_controller::battle_controller(battle b, team t)
    : state_{start_battle(std::move(b), std::move(t))}
{
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
