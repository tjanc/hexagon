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
    static std::size_t& next_team_id()
    {
        static std::size_t next_id_ = 0;
        return next_id_;
    }

    static std::size_t unique_team_id()
    {
        std::size_t id = next_team_id();
        ++next_team_id();
        return id;
    }

    void respond_specific(shared_state& ss, websocket_session& source,
                          connecting_state& s, const login_request& request)
    {
        auto id = unique_team_id();
        std::cout << "Someone is logging in as " << request.name
                  << ", giving them team " << id << '\n';

        const world_state& in_world = source.local().to_world(world_state{
            world{team{id, team::unit_container{
                               unit{0 + id * 10, unit_job::warrior},  //
                               unit{1 + id * 10, unit_job::mage}      //
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
