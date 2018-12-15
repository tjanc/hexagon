//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "connecting_controller.hpp"

#include <hexagon/model/team.hpp>
#include <hexagon/model/world.hpp>
#include <hexagon/protocol/login_request.hpp>
#include <iostream>
#include "connection.hpp"
#include "game_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

connecting_controller::connecting_controller(connecting_facet facet) noexcept
    : model_{}, facet_{std::move(facet)}
{
}

void connecting_controller::update(game_controller& c, const mouse& m) noexcept
{
}

void connecting_controller::draw(canvas& c) const { facet_.draw(c, model_); }

void connecting_controller::update(game_controller& c, version_response m)
{
    std::cout << "Connected to server running version: " << m << '\n';
    // TODO test client/server version comopatibility

    std::cout << "Log in as Johnny\n";
    login_request request{"Johnny"};
    connection::instance().async_send(request);
}

void connecting_controller::update(game_controller& c, login_response m)
{
    std::cout << "Logged in with key " << m.key << '\n';
    // session_key_ = msg.key;
    c.to_world(world{team{}});
}
