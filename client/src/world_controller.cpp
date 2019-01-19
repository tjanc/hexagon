//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "world_controller.hpp"

#include <iostream>
#include "game_controller.hpp"
#include "graphics.hpp"
#include "mouse.hpp"

#include <hexagon/protocol/battle_message.hpp>

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

world_controller::world_controller(world m, world_facet facet) noexcept
    : model_{std::move(m)}, facet_{std::move(facet)}
{
}

void world_controller::update(game_controller& c, const mouse& m) noexcept {}

void world_controller::draw(graphics& c) const { facet_.draw(c, model_); }

void world_controller::update(game_controller& c, battle_message m)
{
    std::cout << "Into battle! There are " << m.battle.teams().size()
              << " teams, you are number " << m.team_id << '\n';

    if (m.team_id >= m.battle.teams().size()) {
        std::cerr << "WARN: invalid team id `" << m.team_id << "`\n";
        return;
    }

    c.to_battle(battle_controller{
        std::move(m.battle),                                  //
        battle_facet{0, 0, facet_.width(), facet_.height()},  //
        m.team_id});
}

world_facet& world_controller::facet() noexcept { return facet_; }
