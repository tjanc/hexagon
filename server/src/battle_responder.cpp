//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_responder.hpp"

#include <hexagon/protocol/battle_request.hpp>
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
    constexpr bool responding_state<world_state> = true;

}  // namespace

namespace
{
    void respond_specific(shared_state& ss, websocket_session& source,
                          world_state& s, const battle_request& request)
    {
        const auto& t = s.raw().team_;
        auto b = battle{*ss.assets().get_map(0)};

        const auto team_id = b.join(t).id;

        const battling_state& in_battle = source.local().to_battle(
            battling_state{std::move(b), team_id});

        std::string msg;
        write_message<battle_message>(msg, 0, in_battle.get_battle());
        source.send(std::make_shared<std::string>(std::move(msg)));
    }
}  // namespace

void hexagon::server::respond(shared_state& ss, websocket_session& ws,
                              const battle_request& msg)
{
    std::cerr << "Received battle request\n";

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
