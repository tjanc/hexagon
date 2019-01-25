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
        auto& lobby = ss.lobby();
        auto joined = lobby.join(source, s);

        team* my_team = joined.first;
        auto& my_placements = joined.second;

        const battling_state& in_battle = source.local().to_battle(
            battling_state{lobby.battle(), my_team->id});

        if (lobby.players().size() > 1) {
            std::string msg;
            write_message<joined_battle_message>(msg, *my_team, my_placements);
            for (websocket_session* player : lobby.players()) {
                if (player != &source) {
                    auto& player_state =
                        std::get<battling_state>(player->local().raw());

                    auto& b = player_state.get_battle();
                    b.join(*my_team, my_placements);
                    if (player_state.joining() && b.ready())
                        player_state.start();

                    player->send(std::make_shared<std::string>(msg));
                }
            }
        }

        {
            std::string msg;
            write_message<battle_message>(msg, my_team->id,
                                          in_battle.get_battle());
            source.send(std::make_shared<std::string>(std::move(msg)));
        }
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
