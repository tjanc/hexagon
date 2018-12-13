//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "shared_state.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

#include <hexagon/model/map_loader.hpp>
#include <hexagon/protocol/message.hpp>
#include "websocket_session.hpp"

using boost::system::error_code;
using namespace hexagon::server;

namespace
{
    using namespace hexagon::protocol;

    void handle_client_message(const map_request& request,
                               websocket_session& source,
                               const preload_assets& assets)
    {
        using namespace hexagon::model;

        std::cout << "A user requested map " << request.id << "\n";

        std::string msg;
        if (const auto* m = assets.get_map(request.id)) {
            {
                map_response response{*m};
                write_message(msg, response);
            }
        } else {
            map_response response{};
            write_message(msg, response);
        }
        source.send(std::make_shared<std::string>(std::move(msg)));
    }

    void handle_client_message(const login_request& request,
                               websocket_session& source,
                               const preload_assets& assets)
    {
        using namespace hexagon::model;

        std::cout << "Someone is logging in as " << request.name << "\n";

        // TODO login logic, returning dummy session key

        {
            login_response response{"42"};

            std::string msg;
            write_message(msg, response);
            source.send(std::make_shared<std::string>(std::move(msg)));
        }

        {
            const auto* m = assets.get_map(0);
            assert(m);
            battle b{*m};

            b.join(team{1, team::unit_container{
                               unit{1},  //
                               unit{2}   //
                           }});
            battle_message message{std::move(b), 0};

            std::string msg;
            write_message(msg, message);
            source.send(std::make_shared<std::string>(std::move(msg)));
        }
    }

    void handle_client_message(const unknown_message&, websocket_session&,
                               const preload_assets& assets)
    {
        std::cerr << "Received unknown_message\n";
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
    hexagon::protocol::version_response response{0, 0, 2};

    std::string msg;
    write_message(msg, response);
    session.send(std::make_shared<std::string>(std::move(msg)));
}

void shared_state::leave(websocket_session& session)
{
    sessions_.erase(&session);
}
