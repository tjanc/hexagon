//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battling_adapter.hpp"

#include <hexagon/protocol/io/message_io.hpp>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/team.hpp>

#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>

#include <hexagon/state/battling_state.hpp>

#include "battle_facet.hpp"
#include "connection.hpp"
#include "game_facet.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::state;
using namespace hexagon::protocol;

namespace
{
    template <typename Message>
    constexpr bool handles_message = false;

    template <>
    constexpr bool handles_message<move_message> = true;

}  // namespace

namespace
{
    void update_specific(battling_state& s, units_moved& model,
                         battle_facet& facet, const mouse& m)
    {
        //
    }

    void mouse_moved(const mouse& m, battle_facet& facet, battle& b)
    {
        auto idx = facet.transpose(m.x, m.y);
        if (contains(b.get_map(), idx)) facet.map().hover(idx);
    }

    void update_specific(battling_state& s, unit_moving& model,
                         battle_facet& facet, const mouse& m)
    {
        auto source = model.position();
        auto target = facet.map().hover();

        if (model.reachable(s.get_battle().get_map(), target)) {
            connection::instance().async_send<move_request>(source, target);

            if (model.has_next())
                model.next(s.get_battle());
            else
                s.raw() = units_moved{std::move(model)};
        }
    }
}  // namespace

namespace
{
    void update_specific(local_state& s, battling_state& cstate,
                         game_facet& facet, move_message m)
    {
        move_unit(cstate.get_battle().get_map(), m.source, m.target);
    }
}  // namespace

void hexagon::client::update(local_state& s, battling_state& cstate,
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

void hexagon::client::update(local_state&, battling_state& cstate,
                             game_facet& gfacet, const mouse& m)
{
    auto& facet = gfacet.get(cstate);

    if (m.moved()) {
        mouse_moved(m, facet, cstate.get_battle());
    }

    if (m.released()) {
        std::visit(
            [&m, &state = cstate, &facet](auto& s) {  //
                update_specific(state, s, facet, m);
            },
            cstate.raw());
    }
}
