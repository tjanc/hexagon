//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "connecting_adapter.hpp"

#include <hexagon/model/world.hpp>
#include <hexagon/state/local_state.hpp>
#include "battling_adapter.hpp"
#include "game_facet.hpp"
#include "mouse.hpp"
#include "world_adapter.hpp"

#include "connection.hpp"

using namespace hexagon::client;
using namespace hexagon::protocol;
using namespace hexagon::state;
using namespace hexagon::model;

namespace
{
    template <typename Message>
    constexpr bool handles_message = false;

    template <>
    constexpr bool handles_message<version_response> = true;

    template <>
    constexpr bool handles_message<world_message> = true;
}  // namespace

namespace
{
    void update_specific(local_state& s, connecting_state& cstate,
                         game_facet& facet, version_response m)
    {
        std::cout << "Connected to server running version: " << m << '\n';
        // TODO test client/server version comopatibility

        std::cout << "Log in as Johnny\n";
        connection::instance().async_send<login_request>("Johnny");
    }

    void update_specific(local_state& s, connecting_state& cstate,
                         game_facet& facet, world_message m)
    {
        std::cout << "Entering world!\n";

        s.to_world(world_state{world{std::move(m.team)}});
        facet = world_facet{0, 0, facet.width(), facet.height()};

        std::cout << "DEBUG: explicitly asking for battle\n";
        connection::instance().async_send<battle_request>(0);
    }
}  // namespace

void hexagon::client::update(local_state& s, connecting_state& cstate,
                             game_facet& facet, protocol::server_message msg)
{
    std::visit(
        [&s, &cstate, &facet](auto m) {
            using M = typename std::decay<decltype(m)>::type;
            if constexpr (handles_message<M>) {
                update_specific(s, cstate, facet, std::move(m));
            } else {
                std::cout << "WARN: ignoring unexpected message "
                          << id<M> << '\n';
            }
        },
        std::move(msg));
}

void hexagon::client::update(local_state&, connecting_state&, game_facet&,
                             const mouse&)
{
    // don't react to mouse when connecting
}
