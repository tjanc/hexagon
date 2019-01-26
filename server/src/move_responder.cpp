//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "move_responder.hpp"

#include <hexagon/protocol/message.hpp>
#include <hexagon/protocol/move_request.hpp>

#include "shared_state.hpp"
#include "websocket_session.hpp"

#include <iostream>
#include <set>

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
        std::cout << "TODO: validate source of movement and detect static "
                     "collisions\n";

        auto& b = bstate.get_battle();
        auto& m = b.get_map();

        if (cstate.reachable(m, request.target)) {
            cstate.move(m, request.target);

            if (cstate.has_next())
                cstate.next(b);
            else {
                std::cout
                    << "TODO: check if last player to commit, notify others\n";

                bstate.raw() = units_moved{std::move(cstate)};

                const auto& ps = ss.lobby().players();
                if (ps.end() ==
                    std::find_if(ps.begin(), ps.end(),
                                 [](const websocket_session* s) {  //
                                     assert(s);
                                     const auto* bs =
                                         std::get_if<battling_state>(
                                             &s->local().raw());
                                     if (!bs) return true;
                                     const auto* m =
                                         std::get_if<units_moved>(&bs->raw());
                                     return !m;
                                 })) {
                    std::cout << "INFO: replaying all moves\n";

                    std::set<movement> all_moves;
                    for (const websocket_session* s : ps) {
                        assert(s);
                        const auto& bs =
                            std::get<battling_state>(s->local().raw());
                        const auto& m = std::get<units_moved>(bs.raw());
                        std::cout << "INFO: === mergin player with #"
                                  << m.movements().size() << " moves\n";
                        std::copy(m.movements().begin(), m.movements().end(),
                                  std::inserter(all_moves, all_moves.begin()));
                    }

                    auto& central_map = ss.lobby().battle().get_map();
                    for (const movement& mov : all_moves) {
                        std::cout << "INFO: === delay: "
                                  << std::chrono::duration_cast<
                                         std::chrono::milliseconds>(mov.delay)
                                         .count()
                                  << "ms;\tsource: " << mov.source.x << 'x'
                                  << mov.source.y
                                  << ";\ttarget: " << mov.target.x << 'x'
                                  << mov.target.y << ";\n";

                        if (central_map.at(mov.target).empty()) {
                            move_unit(central_map, mov.source, mov.target);
                            std::string msg;
                            write_message<move_message>(msg, mov.source,
                                                        mov.target);

                            for (websocket_session* player : ps) {
                                auto& pstate = std::get<battling_state>(
                                    player->local().raw());
                                move_unit(pstate.get_battle().get_map(), mov.source,
                                          mov.target);
                                player->send(
                                    std::make_shared<std::string>(msg));
                            }
                        } else {
                            std::cerr << "TODO: resolve movement collision\n";
                        }
                    }
                }
            }
        }
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
