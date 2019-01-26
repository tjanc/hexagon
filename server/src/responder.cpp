//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "responder.hpp"

#include <hexagon/protocol/message.hpp>

#include "shared_state.hpp"
#include "websocket_session.hpp"

#include "battle_responder.hpp"
#include "login_responder.hpp"
#include "move_responder.hpp"

#include <type_traits>

using namespace hexagon::model;
using namespace hexagon::protocol;
using namespace hexagon::server;
using namespace hexagon::state;

void hexagon::server::respond(shared_state& ss, websocket_session& ws,
                              const std::string& m)
{
    const auto msg = hexagon::protocol::read_client_message(m);

    std::visit(
        [&ws, &ss](const auto& m) {
            using M = typename std::decay<decltype(m)>::type;
            if constexpr (std::is_same<M, unknown_message>::value)
                std::cerr << "Received unknown_message: `" << m.id << ' '
                          << m.content << "`\n";
            else
                respond(ss, ws, m);
        },
        msg);
}
