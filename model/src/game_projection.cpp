//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/game_projection.hpp>

using namespace hexagon::model;

game_projection::game_projection(battle& b, team t)
    : state_{battle_projection{b, std::move(t)}}
{
}
