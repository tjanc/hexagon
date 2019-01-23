//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "shared_state.hpp"

#include <hexagon/protocol/message.hpp>
#include <hexagon/state/battling_state.hpp>
#include <hexagon/state/world_state.hpp>
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
    hexagon::protocol::write_message<hexagon::protocol::version_response>(
        msg, 0, 0, 2);
    session.send(std::make_shared<std::string>(std::move(msg)));
}

void shared_state::leave(websocket_session& session)
{
    sessions_.erase(&session);
}

shared_state::shared_state(preload_assets assets, std::string root)
    : assets_(std::move(assets)),
      lobby_{model::battle{*assets_.get_map(0)}},
      document_root_(std::move(root))
{
}

using namespace hexagon::state;

battle_lobby::battle_lobby(model::battle b) : battle_{std::move(b)} {}

std::size_t battle_lobby::join(websocket_session& ws, const world_state& s)
{
    std::cout << "INFO: player joining battle lobby\n";
    players_.insert(&ws);
    return battle_.join(s.raw().team_);
}

void battle_lobby::leave(websocket_session& ws, const battling_state& s)
{
    std::cout << "INFO: player leaving battle lobby\n";
    auto t = battle_.leave(s.team_id());
    std::cout << "TODO: persist team after leaving battle lobby\n";
    players_.erase(&ws);
}
