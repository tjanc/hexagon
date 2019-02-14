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
        if (!cstate.follow(b.get_map(), request.route)) {
            cstate.next(b.get_map());  // explicitly skip moves not making sense
        }

        if (cstate.end()) {
            bstate.raw() = units_moved{std::move(cstate)};

            const auto& ps = ss.lobby().players();
            if (ps.end() ==
                std::find_if(ps.begin(), ps.end(),
                             [](const websocket_session* s) {  //
                                 assert(s);
                                 const auto* bs = std::get_if<battling_state>(
                                     &s->local().raw());
                                 if (!bs) return true;
                                 const auto* m =
                                     std::get_if<units_moved>(&bs->raw());
                                 return !m;
                             })) {
                std::cout << "INFO: last to commit moves, replaying\n";

                std::set<movement> all_moves;
                for (const websocket_session* s : ps) {
                    assert(s);
                    const auto& bs = std::get<battling_state>(s->local().raw());
                    const auto& m = std::get<units_moved>(bs.raw());
                    std::copy(m.movements().begin(), m.movements().end(),
                              std::inserter(all_moves, all_moves.begin()));
                }

                auto& central_map = ss.lobby().battle().get_map();
                for (const movement& mov : all_moves) {

                    std::cerr << "YYY: " << mov.route << '\n';

                    const auto source = mov.route.source();
                    const auto [tgt, stamina] =
                        traverse_unit(central_map, source, mov.route.end());
                    if (tgt == mov.route.end()) continue;

                    std::cerr << "ZZZ: " << tgt->x << ' ' << tgt->y << '\n';

                    vertex_path response_path;
                    std::copy(mov.route.source(), tgt + 1,
                              std::back_inserter(response_path.vertices()));

                    std::cerr << "XXXDEBUGG: " << response_path << '\n';

                    std::string msg;
                    write_message<move_message>(msg, response_path, stamina);
                    std::cerr << "XXXDEBUGG: " << msg << '\n';

                    for (websocket_session* player : ps) {
                        auto& pstate =
                            std::get<battling_state>(player->local().raw());
                        auto& m = pstate.get_battle().get_map();
                        move_unit(m, *source, *tgt, stamina);
                        player->send(std::make_shared<std::string>(msg));
                    }
                }
            }
        }
    }  // namespace

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
