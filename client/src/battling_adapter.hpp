//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLING_ADAPTER_H_
#define HEXAGON_CLIENT_BATTLING_ADAPTER_H_

#include <hexagon/protocol/message.hpp>

namespace hexagon::client
{
    class mouse;
    class game_facet;
}  // namespace hexagon::client

namespace hexagon::state
{
    class local_state;
    class battling_state;
}

namespace hexagon::client
{
    void update(state::local_state&, state::battling_state&, game_facet&, protocol::server_message);
    void update(state::local_state&, state::battling_state&, game_facet&, const mouse&);
}  // namespace hexagon::client

#endif
