//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "shared_state.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map_loader.hpp>
#include <hexagon/protocol/io/battle_io.hpp>
#include <hexagon/protocol/message.hpp>
#include "websocket_session.hpp"

using boost::system::error_code;
using namespace hexagon::server;

namespace
{
    using namespace hexagon::protocol;
    using namespace hexagon::model;

    void handle_client_message(const map_request& request,
                               websocket_session& source, battle& b)
    {
    }

    void handle_client_message(const login_request& request,
                               websocket_session& source, battle& b)
    {
        using namespace hexagon::model;

        std::cout << "Someone is logging in as " << request.name << "\n";

        // TODO login logic, returning dummy session key

        {
            std::string msg;
            write_message<login_response>(msg, "42");
            source.send(std::make_shared<std::string>(std::move(msg)));
        }

        {
            b.join(team{1, team::unit_container{
                               unit{0, unit_job::warrior},  //
                               unit{1, unit_job::mage}      //
                           }});

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

    void handle_client_message(const move_request& request,
                               websocket_session& source, battle& b)
    {
        std::cout << "Received movement message\n";
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
