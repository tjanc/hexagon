//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "shared_state.hpp"

#include <hexagon/state/local_state.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map_loader.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/protocol/io/battle_io.hpp>
#include <hexagon/protocol/message.hpp>
#include <hexagon/state/battling_state.hpp>
#include <hexagon/state/connecting_state.hpp>
#include <hexagon/state/world_state.hpp>
#include "websocket_session.hpp"

using boost::system::error_code;
using namespace hexagon::server;

namespace
{
    using namespace hexagon::protocol;
    using namespace hexagon::model;
    using namespace hexagon::state;

    void handle_login_request(const login_request&, world_state&,
                              websocket_session&, const preload_assets&)
    {
        std::cerr << "WARN: unexpected login request in world state\n";
    }

    void handle_login_request(const login_request&, battling_state&,
                              websocket_session&, const preload_assets&)
    {
        std::cerr << "WARN: unexpected login request in battling state\n";
    }

    void handle_login_request(const login_request& request, connecting_state& s,
                              websocket_session& source,
                              const preload_assets& assets)
    {
        std::cout << "Someone is logging in as " << request.name << "\n";
        {
            // TODO login logic, returning dummy session key

            source.local().to_world(world{team{}});

            std::string msg;
            write_message<login_response>(msg, "42");
            source.send(std::make_shared<std::string>(std::move(msg)));
        }

        {
            const battling_state& in_battle = source.local().to_battle(
                battle{*assets.get_map(0)},
                team{0, team::unit_container{
                            unit{0, unit_job::warrior},  //
                            unit{1, unit_job::mage}      //
                        }});

            std::string msg;
            write_message<battle_message>(msg, 0, in_battle.get_battle());
            source.send(std::make_shared<std::string>(std::move(msg)));
        }
    }

    void handle_client_message(const login_request& request,
                               websocket_session& source,
                               const preload_assets& assets)
    {
        std::cerr << "Received login request\n";
        std::visit(
            [&request, &source, &assets](auto& local) {
                handle_login_request(request, local, source, assets);
            },
            source.local().raw());
    }

    void handle_client_message(const unknown_message&, websocket_session&,
                               const preload_assets& assets)
    {
        std::cerr << "Received unknown_message\n";
    }

    void handle_move_request_specific(const move_request&, units_moved&,
                                      websocket_session&, battle&)
    {
        std::cerr << "WARN: unexpected move request in units_moved state\n";
    }

    void handle_move_request_specific(const move_request& request,
                                      unit_moving& cstate,
                                      websocket_session& source, battle& b)
    {
        std::cout << "Correct state, handling movement message\n";

        if (cstate.reachable(b.get_map(), request.target)) {
            std::cout << "=== units on battlefield: "
                      << std::count_if(
                             b.get_map().begin(), b.get_map().end(),
                             [](const auto& el) { return el.has_unit(); });
            cstate.move(b.get_map(), request.target);
            // emit<move_request>(source, request.target);

            if (cstate.has_next()) cstate.next(b);
            // else
            //    s = units_moved{std::move(cstate)};
        }

        std::cout << "TODO: check if last player to commit, notify others\n";
    }

    void handle_move_request(const move_request&, connecting_state&,
                             websocket_session&)
    {
        std::cerr << "WARN: unexpected move request in connecting state\n";
    }

    void handle_move_request(const move_request&, world_state&,
                             websocket_session&)
    {
        std::cerr << "WARN: unexpected move request in world state\n";
    }

    void handle_move_request(const move_request& request,
                             battling_state& cstate, websocket_session& source)
    {
        std::visit(
            [&request, &source, &b = cstate.get_battle()](auto& cstate) {
                handle_move_request_specific(request, cstate, source, b);
            },
            cstate.raw());
    }

    void handle_client_message(const move_request& request,
                               websocket_session& source,
                               const preload_assets& assets)
    {
        std::cout << "Received movement message\n";

        std::visit(
            [&request, &source](auto& cstate) {
                handle_move_request(request, cstate, source);
            },
            source.local().raw());
    }
}  // namespace

void shared_state::handle(std::string message, websocket_session& source)
{
    auto msg = hexagon::protocol::read_client_message(message);

    std::visit(
        [&source, &assets = assets_](const auto& m) {  //
            handle_client_message(m, source, assets);
        },
        msg);
}

void shared_state::join(websocket_session& session)
{
    sessions_.insert(&session);

    std::string msg;
    write_message<version_response>(msg, 0, 0, 2);
    session.send(std::make_shared<std::string>(std::move(msg)));
}

void shared_state::leave(websocket_session& session)
{
    sessions_.erase(&session);
}

shared_state::shared_state(preload_assets assets, std::string root)
    : assets_(std::move(assets)),
      test_battle_{*assets_.get_map(0)},
      document_root_(std::move(root))
{
}
