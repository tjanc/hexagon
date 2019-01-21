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
                              websocket_session&, shared_state& ss)
    {
        std::cerr << "WARN: unexpected login request in world state\n";
    }

    void handle_login_request(const login_request&, battling_state&,
                              websocket_session&, shared_state& ss)
    {
        std::cerr << "WARN: unexpected login request in battling state\n";
    }

    void handle_login_request(const login_request& request, connecting_state& s,
                              websocket_session& source, shared_state& ss)
    {
        std::cout << "Someone is logging in as " << request.name << "\n";

        const world_state& in_world = source.local().to_world(
            world_state{world{team{0, team::unit_container{
                                          unit{0, unit_job::warrior},  //
                                          unit{1, unit_job::mage}      //
                                      }}}});

        std::string msg;
        write_message<world_message>(msg, in_world.raw().team_);
        source.send(std::make_shared<std::string>(std::move(msg)));
    }

    void handle_client_message(const login_request& request,
                               websocket_session& source, shared_state& ss)
    {
        std::cerr << "Received login request\n";
        source.local().update([&request, &source, &ss](auto& local) {
            handle_login_request(request, local, source, ss);
        });
    }

    void handle_battle_request(const battle_request&, connecting_state&,
                               websocket_session&, shared_state& ss)
    {
        std::cerr << "WARN: unexpected battle request in connecting state\n";
    }

    void handle_battle_request(const battle_request&, battling_state&,
                               websocket_session&, shared_state& ss)
    {
        std::cerr << "WARN: unexpected battle request in battling state\n";
    }

    void handle_battle_request(const battle_request& request, world_state& s,
                               websocket_session& source, shared_state& ss)
    {
        const auto& t = s.raw().team_;
        auto b = battle{*ss.assets().get_map(0)};

        auto idx = b.join(t);

        const battling_state& in_battle =
            source.local().to_battle(battling_state{std::move(b), idx});

        std::string msg;
        write_message<battle_message>(msg, 0, in_battle.get_battle());
        source.send(std::make_shared<std::string>(std::move(msg)));
    }

    void handle_client_message(const battle_request& request,
                               websocket_session& source, shared_state& ss)
    {
        std::cerr << "Received battle request\n";
        source.local().update([&request, &source, &ss](auto& local) {
            handle_battle_request(request, local, source, ss);
        });
    }

    void handle_client_message(const unknown_message&, websocket_session&,
                               shared_state& ss)
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

        std::cout << "TODO: validate source of movement and detect static "
                     "collisions\n";
        if (cstate.reachable(b.get_map(), request.target)) {
            cstate.move(b.get_map(), request.target);

            std::string msg;
            write_message<move_message>(msg, request.source, request.target);
            source.send(std::make_shared<std::string>(std::move(msg)));

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
                               websocket_session& source, shared_state& ss)
    {
        std::cout << "Received movement request\n";

        source.local().update([&request, &source](auto& cstate) {
            handle_move_request(request, cstate, source);
        });
    }
}  // namespace

void shared_state::handle(std::string message, websocket_session& source)
{
    auto msg = hexagon::protocol::read_client_message(message);

    std::visit(
        [&source, &ss = *this](const auto& m) {  //
            handle_client_message(m, source, ss);
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
