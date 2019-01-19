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

namespace {
    template <typename Message, typename... Args>
    decltype(auto) emit(Args&&... args)
    {
        return connection::instance().async_send<Message>(
            std::forward<Args>(args)...);
    }
}

namespace
{
    void update_specific(battle_controller::state& s, battle& b,
                         units_moved& model, const battle_facet& facet,
                         const mouse& m)
    {
        //
    }

    void mouse_moved(const mouse& m, battle_facet& facet, battle& b)
    {
        auto idx = facet.transpose(m.x, m.y);
        if (contains(b.get_map(), idx)) facet.map().hover(idx);
    }

    void update_specific(battle_controller::state& s, battle& b,
                         unit_moving& model, battle_facet& facet,
                         const mouse& m)
    {
        auto source = model.position();
        auto target = facet.map().hover();

        if (model.reachable(b.get_map(), target)) {
            model.move(b.get_map(), target);
            emit<move_request>(source, target);

            if (model.has_next())
                model.next(b);
            else
                s = units_moved{std::move(model)};
        }
    }
}  // namespace

battle_controller::battle_controller(battle b, battle_facet facet,
                                     std::size_t team_index) noexcept
    : battle_{std::move(b)},
      state_{unit_moving{battle_, team_index}},
      facet_{std::move(facet)}
{
    std::cout << "Battle loaded, controlling team " << team_index << ".\n";
}

void battle_controller::update(const mouse& m) noexcept
{
    if (m.moved()) {
        mouse_moved(m, facet_, battle_);
    }

    if (m.released()) {
        std::visit(
            [&m, &state = state_, &facet = facet_, &b = battle_](auto& s) {  //
                update_specific(state, b, s, facet, m);
            },
            state_);
    }
}

void battle_controller::draw(graphics& c) const
{
    std::visit(
        [&c, &facet = facet_, &m = battle_.get_map()](const auto& s) {  //
            facet.draw(c, m, s);
        },
        state_);
}

battle_facet& battle_controller::facet() noexcept { return facet_; }
