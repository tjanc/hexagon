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

    template <>
    constexpr bool handles_message<joined_battle_message> = true;

}  // namespace

namespace
{
    bool find_path_step(vertex_path& result, const reach_map& reaches,
                        basic_map_index src, basic_map_index tgt)
    {
        std::cout << "INFO: path step " << src.x << 'x' << src.y << '/' << tgt.x
                  << 'x' << tgt.y << '\n';

        if (!contains(reaches, tgt) || !contains(reaches, src)) return false;

        const auto tgt_reach = reaches.at(tgt);
        const auto src_reach = reaches.at(src);

        if (src_reach <= 0 || tgt_reach <= 0 || src_reach < tgt_reach)
            return false;

        result.vertices().emplace_front(tgt);

        if (src == tgt) return true;

        std::array<basic_map_index, 6> cs = {
            west(tgt),        //
            north_west(tgt),  //
            north_east(tgt),  //
            east(tgt),        //
            south_west(tgt),  //
            south_east(tgt)   //
        };

        std::sort(cs.begin(), cs.end(),
                  [&reaches](const auto& lhs, const auto& rhs) {
                      return reaches.at(lhs) > reaches.at(rhs);
                  });

        for (const auto& c : cs)
            if (reaches.at(c) > tgt_reach &&
                find_path_step(result, reaches, src, c))
                return true;

        return false;
    }

    vertex_path find_path(const reach_map& reaches, basic_map_index src,
                          basic_map_index tgt)
    {
        vertex_path result{};
        if (!find_path_step(result, reaches, src, tgt))
            std::cerr << "WARN: no path found from "      //
                      << src.x << 'x' << src.y << " to "  //
                      << tgt.x << 'x' << tgt.y << '\n';
        return result;
    }
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
        const auto source = model.position();
        const auto target = facet.map().hover();

        auto& field = s.get_battle().get_map();
        auto path = find_path(model.reaches(), source, target);
        std::cout << "INFO: found path: " << path << '\n';
        if (model.follow(field, path)) {
            connection::instance().async_send<move_request>(std::move(path));
        }
        if (model.end()) s.raw() = units_moved{std::move(model)};
    }
}  // namespace

namespace
{
    void update_specific(local_state& s, battling_state& cstate,
                         game_facet& facet, move_message m)
    {
        auto& field = cstate.get_battle().get_map();

        if (!move_unit(field, *m.route.source(), *m.route.target(), m.cost)) {
            std::cerr << "ERROR: invalid move\n";
            return;
        }
    }

    void update_specific(local_state& s, battling_state& cstate,
                         game_facet& facet, joined_battle_message m)
    {
        auto& b = cstate.get_battle();
        b.join(m.team, m.placements);

        if (cstate.joining() && b.ready()) {
            cstate.start();
        }
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
