//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game_controller.hpp"

#include <hexagon/model/map_loader.hpp>
#include "connection.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

namespace
{
    battle battle_with_opponent(map m)
    {
        battle b{std::move(m)};
        b.join(team{});
        return b;
    }
}  // namespace

game_controller::game_controller() : state_{connecting_controller{}}
{
    enter_battle(battle_with_opponent(load_map("assets/map1.map")), team{});
}

void game_controller::enter_battle(battle b, team t)
{
    state_ = battle_controller{std::move(b), std::move(t)};
}

void game_controller::operator()(const version_response& msg)
{
    std::cout << "Connected to server running version: " << msg << '\n';
    map_request request{0};
    connection::instance().async_send(request);
}

void game_controller::operator()(const map_response& msg)
{
    std::cout << "Updating map\n";
    enter_battle(battle_with_opponent(msg.map), team{});
}

void game_controller::operator()(const unknown_message& msg)
{
    std::cerr << "Unknown message `" << msg.id << " " << msg.content << "`\n";
}
