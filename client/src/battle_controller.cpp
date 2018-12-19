//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_controller.hpp"

#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/move_request.hpp>

#include <hexagon/protocol/io/std_io.hpp>

#include <iostream>
#include "connection.hpp"
#include "mouse.hpp"


using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

namespace
{
    void update_specific(battle_controller::model& state, units_moved& model,
                         const battle_facet& facet, const mouse& m)
    {
        //
    }

    // TODO make battle_facet const&
    void update_specific(battle_controller::model& state, unit_moving& model,
                         battle_facet& facet, const mouse& m)
    {
        map& field = model.battlefield().get_map();

        if (m.moved()) {
            auto idx = facet.transpose(m.x, m.y);

            if (contains(field, idx)) facet.map().hover(idx);
        }

        if (m.released()) {
            auto source = model.position();
            auto target = facet.map().hover();

            assert(contains(field, source));
            if (model.reachable(target)) {
                model.move(target);

                if (!model.has_next()) {
                    auto cmds = model.commands();
                    state = units_moved{std::move(model)};
                    connection::instance().async_send<move_request>(cmds);
                } else {
                    model.next();
                }
            }
        }
    }
}  // namespace

battle_controller::battle_controller(battle_facet facet, battle b,
                                     std::size_t team_index) noexcept
    : state_{unit_moving{std::move(b), team_index}}, facet_{std::move(facet)}
{
    std::cout << "Battle loaded, controlling team " << team_index << ".\n";
}

void battle_controller::update(const mouse& m) noexcept
{
    std::visit(
        [&m, &state = state_, &facet = facet_](auto& s) {  //
            update_specific(state, s, facet, m);
        },
        state_);
}

void battle_controller::draw(graphics& c) const
{
    std::visit(
        [&c, &facet = facet_](const auto& m) {  //
            facet.draw(c, m);
        },
        state_);
}

battle_facet& battle_controller::facet() noexcept { return facet_; }
