//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "move_responder.hpp"

#include <hexagon/protocol/message.hpp>
#include <hexagon/protocol/move_request.hpp>

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
    constexpr bool responding_state<battling_state> = true;

}  // namespace

namespace
{
    void respond_specific(shared_state& ss, websocket_session& source,
                          battling_state& bstate, unit_moving& cstate,
                          const move_request& request)
    {
        std::cout << "Correct state, handling movement message\n";

        std::cout << "TODO: validate source of movement and detect static "
                     "collisions\n";

        auto& b = bstate.get_battle();
        auto& m = b.get_map();

        if (cstate.reachable(m, request.target)) {
            cstate.move(m, request.target);

            std::string msg;
            write_message<move_message>(msg, request.source, request.target);
            source.send(std::make_shared<std::string>(std::move(msg)));

            if (cstate.has_next())
                cstate.next(b);
            else {
                bstate.raw() = units_moved{std::move(cstate)};
            }
        }

        std::cout << "TODO: check if last player to commit, notify others\n";
    }

    void respond_specific(shared_state& ss, websocket_session& source,
                          battling_state& bstate, const move_request& request)
    {
        std::visit(
            [&ss, &source, &bstate, &request](auto& s) {
                using S = typename std::decay<decltype(s)>::type;
                if constexpr (std::is_same<S, unit_moving>::value)
                    respond_specific(ss, source, bstate, s, request);
                else
                    std::cerr << "WARN: unexpected move request in units_moved "
                                 "state\n";
            },
            bstate.raw());
    }
}  // namespace

void hexagon::server::respond(shared_state& ss, websocket_session& ws,
                              const move_request& msg)
{
    std::cerr << "Received move request\n";

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
