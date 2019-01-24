//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battling_adapter.hpp"

#include <hexagon/protocol/io/message_io.hpp>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/team.hpp>

#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_joining.hpp>
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
    void mouse_released(battling_state& s, units_joining& model,
                        battle_facet& facet, const mouse& m)
    {
        //
    }

    void mouse_released(battling_state& s, units_moved& model,
                        battle_facet& facet, const mouse& m)
    {
        //
    }

    void mouse_released(battling_state& s, unit_moving& model,
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
        auto& field = cstate.get_battle().get_map();

        auto* src = field.at(m.source).get_if_unit();
        if (!src) {
            std::cerr << "ERROR: invalid move message; no unit at source\n";
            return;
        }

        std::cout << "INFO: moving unit " << src->id() << " from "  //
                  << m.source.x << 'x' << m.source.y << " to "      //
                  << m.target.x << 'x' << m.target.y << '\n';

        move_unit(field, m.source, m.target);
    }
}  // namespace

void hexagon::client::update(local_state& s, battling_state& cstate,
                             game_facet& facet, protocol::server_message&& msg)
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
        const auto idx = transpose(facet, cstate, m.x, m.y);
        if (contains(cstate.get_battle().get_map(), idx))
            facet.map().hover(idx);
    }

    if (m.released()) {
        std::visit(
            [&m, &cstate, &facet](auto& s) {  //
                mouse_released(cstate, s, facet, m);
            },
            cstate.raw());
    }
}
