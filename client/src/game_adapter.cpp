//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game_adapter.hpp"

#include <hexagon/state/local_state.hpp>
#include "battling_adapter.hpp"
#include "connecting_adapter.hpp"
#include "game_facet.hpp"
#include "mouse.hpp"
#include "world_adapter.hpp"

using namespace hexagon::client;
using namespace hexagon::protocol;
using namespace hexagon::state;

void hexagon::client::update(local_state& s, game_facet& facet,
                             server_message msg)
{
    s.update([&s, &facet, m = std::move(msg)](auto& cstate) {
        hexagon::client::update(s, cstate, facet, std::move(m));
    });
}

void hexagon::client::update(local_state& s, game_facet& facet, const mouse& m)
{
    s.update([&s, &m, &facet](auto& cstate) {  //
        return hexagon::client::update(s, cstate, facet, m);
    });
}
