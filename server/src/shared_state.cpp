//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "shared_state.hpp"

#include "local_state.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map_loader.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/protocol/io/battle_io.hpp>
#include <hexagon/protocol/message.hpp>
#include "websocket_session.hpp"

using boost::system::error_code;
using namespace hexagon::server;

namespace
{
    using namespace hexagon::protocol;
    using namespace hexagon::model;

    void handle_client_message(const login_request& request,
                               websocket_session& source, battle& b)
    {
        using namespace hexagon::model;

        std::cout << "Someone is logging in as " << request.name << "\n";

        {
            // TODO login logic, returning dummy session key
        }

        {
            source.local().to_world(world{team{}});

            std::string msg;
            write_message<login_response>(msg, "42");
            source.send(std::make_shared<std::string>(std::move(msg)));
        }

        {
            b.join(team{1, team::unit_container{
                               unit{0, unit_job::warrior},  //
                               unit{1, unit_job::mage}      //
                           }});

            source.local().to_battle(b, 0);

            std::string msg;
            write_message<battle_message>(msg, 0, b);
            source.send(std::make_shared<std::string>(std::move(msg)));
        }
    }

    void handle_client_message(const unknown_message&, websocket_session&,
                               battle& b)
    {
        std::cerr << "Received unknown_message\n";
    }

    void handle_move_request(const move_request&, units_moved&,
                             websocket_session&, battle&)
    {
        std::cerr << "WARN: unexpected move request in units_moved state\n";
    }

    void handle_move_request(const move_request& request, unit_moving& cstate,
                             websocket_session& source, battle& b)
    {
        std::cout << "Correct state, handling movement message\n";
        int i = 1;
        for (const auto& cmd : request.moves) {
            if (cstate.reachable(b.get_map(), cmd.second)) {
                cstate.move(b.get_map(), cmd.second);

                if (cstate.has_next()) {
                    cstate.next(b);
                } else {
                    if (i == request.moves.size()) {
                        std::cout << "INFO: moves accepted\n";
                        break;
                    } else {
                        std::cerr << "WARN: dropping trailing move commands\n";
                        break;
                    }
                }
            } else {
                std::cerr << "WARN: dropping move command not in range\n";
            }
            ++i;
        }

        // TODO check if last player to commit, notify others
    }

    void handle_move_request(const move_request&, connecting&,
                             websocket_session&, battle&)
    {
        std::cerr << "WARN: unexpected move request in connecting state\n";
    }

    void handle_move_request(const move_request&, world&, websocket_session&,
                             battle&)
    {
        std::cerr << "WARN: unexpected move request in world state\n";
    }

    void handle_move_request(const move_request& request,
                             local_state::battling& cstate,
                             websocket_session& source, battle& b)
    {
        std::visit(
            [&request, &source, &b](auto& cstate) {
                handle_move_request(request, cstate, source, b);
            },
            cstate);
    }

    void handle_client_message(const move_request& request,
                               websocket_session& source, battle& b)
    {
        std::cout << "Received movement message\n";

        std::visit(
            [&request, &source, &b](auto& cstate) {
                handle_move_request(request, cstate, source, b);
            },
            source.local().raw());
    }
}  // namespace

void shared_state::handle(std::string message, websocket_session& source)
{
    auto msg = hexagon::protocol::read_client_message(message);

    std::visit(
        [&source, &b = test_battle_](const auto& m) {  //
            handle_client_message(m, source, b);
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
      test_battle_{*assets.get_map(0)},
      document_root_(std::move(root))
{
}
