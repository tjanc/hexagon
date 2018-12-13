//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game_controller.hpp"

#include <hexagon/model/map_loader.hpp>
#include <hexagon/protocol/io/unit_io.hpp>
#include "connection.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

namespace
{
    void update_specific(game_controller& out, connecting_controller&,
                         const map_response& msg)
    {
        // auto b = battle{msg.map};
        // out.state(battle_controller{std::move(b), team{}});
    }

    void update_specific(game_controller&, battle_controller&,
                         const map_response& msg)
    {
    }

    void update_specific(game_controller&, world_controller&,
                         const map_response& msg)
    {
    }

    void update_specific(connecting_controller&, const mouse&) {}

    void update_specific(battle_controller& c, const mouse& m) { c.update(m); }

    void update_specific(world_controller&, const mouse&) {}
}  // namespace

namespace
{
    void draw_specific(const connecting_controller&, canvas&) {}

    void draw_specific(const battle_controller& s, canvas& c) { s.draw(c); }

    void draw_specific(const world_controller&, canvas&) {}
}  // namespace

game_controller::game_controller() : state_{connecting_controller{}}
{
    std::cout << "Starting game...\n";
}

void game_controller::update(version_response msg)
{
    std::cout << "Connected to server running version: " << msg << '\n';
    // TODO test client/server version comopatibility

    std::cout << "Log in as anonymous\n";
    login_request request{"Johnny"};
    connection::instance().async_send(request);
}

void game_controller::update(login_response msg)
{
    std::cout << "Logged in with key " << msg.key << '\n';
    // session_key_ = msg.key;
}

void game_controller::update(map_response msg)
{
    // std::cout << "Updating map\n";
    // std::visit(
    //    [&msg, &self = *this](auto& s) {  //
    //        update_specific(self, s, msg);
    //    },
    //    state_);
    // updated_ = true;
}

void game_controller::update(unknown_message msg)
{
    std::cerr << "Unknown message `" << msg.id << " " << msg.content << "`\n";
}

void game_controller::update(battle_message msg)
{
    std::cout << "Into battle! There are " << msg.battle.teams().size()
              << " teams, you are number " << msg.team_id << '\n';

    if (msg.team_id >= msg.battle.teams().size()) {
        std::cerr << "WARN: invalid team id\n";
        return;
    }

    this->state(battle_controller{std::move(msg.battle), msg.team_id});
}

void game_controller::update(const mouse& m)
{
    std::visit(
        [&m](auto& s) {  //
            return update_specific(s, m);
        },
        state_);
    updated_ = true;
}

void game_controller::draw(canvas& c)
{
    std::visit(
        [&c](const auto& s) {  //
            return draw_specific(s, c);
        },
        state_);
    updated_ = false;
}

bool game_controller::updated() const noexcept { return updated_; }
