//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "shared_state.hpp"

#include <hexagon/protocol/message.hpp>
#include "responder.hpp"
#include "websocket_session.hpp"

using boost::system::error_code;
using namespace hexagon::server;

void shared_state::handle(std::string message, websocket_session& source)
{
    respond(*this, source, message);
}

void shared_state::join(websocket_session& session)
{
    sessions_.insert(&session);

    std::string msg;
    hexagon::protocol::write_message<hexagon::protocol::version_response>(msg, 0, 0, 2);
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
