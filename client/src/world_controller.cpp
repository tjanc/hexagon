//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "world_controller.hpp"

#include <iostream>
#include "canvas.hpp"
#include "game_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

world_controller::world_controller(world m, world_facet facet) noexcept
    : model_{std::move(m)}, facet_{std::move(facet)}
{
}

void world_controller::update(game_controller& c, const mouse& m) noexcept {}

void world_controller::draw(canvas& c) const { facet_.draw(c, model_); }

void world_controller::update(game_controller& c, battle_message m)
{
    std::cout << "Into battle! There are " << m.battle.teams().size()
              << " teams, you are number " << m.team_id << '\n';

    if (m.team_id >= m.battle.teams().size()) {
        std::cerr << "WARN: invalid team id\n";
        return;
    }

    c.to_battle(std::move(m.battle), m.team_id);
}
