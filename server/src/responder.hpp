//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_RESPONDER_H_
#define HEXAGON_SERVER_RESPONDER_H_

#include <string>

namespace hexagon::server
{
    class shared_state;
    class websocket_session;
}  // namespace hexagon::server

namespace hexagon::server
{
    void respond(shared_state&, websocket_session&, const std::string&);
}  // namespace hexagon::server

#endif
