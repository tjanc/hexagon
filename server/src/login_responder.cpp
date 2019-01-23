//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "login_responder.hpp"

#include <hexagon/protocol/login_request.hpp>
#include <hexagon/protocol/message.hpp>

#include "shared_state.hpp"
#include "websocket_session.hpp"

#include <iostream>

using namespace hexagon::model;
using namespace hexagon::protocol;
using namespace hexagon::server;
using namespace hexagon::state;

namespace
{
    template <typename State>
    constexpr bool responding_state = false;

    template <>
    constexpr bool responding_state<connecting_state> = true;

}  // namespace

namespace
{
    void respond_specific(shared_state& ss, websocket_session& source,
                          connecting_state& s, const login_request& request)
    {
        std::cout << "Someone is logging in as " << request.name << "\n";

        const world_state& in_world = source.local().to_world(
            world_state{world{team{0, team::unit_container{
                                          unit{0, unit_job::warrior},  //
                                          unit{1, unit_job::mage}      //
                                      }}}});

        std::string msg;
        write_message<world_message>(msg, in_world.raw().team_);
        source.send(std::make_shared<std::string>(std::move(msg)));
    }
}  // namespace

void hexagon::server::respond(shared_state& ss, websocket_session& ws,
                              const login_request& msg)
{
    std::cerr << "Received login request\n";

    ws.local().update([&msg, &ss, &ws](auto& s) {
        using S = typename std::decay<decltype(s)>::type;
        using M = typename std::decay<decltype(msg)>::type;
        if constexpr (responding_state<S>) {
            respond_specific(ss, ws, s, msg);
        } else {
            std::cout << "WARN: ignoring unexpected message " << id<M> << '\n';
        }
    });
}
