//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game_facet.hpp"

#include <hexagon/state/battling_state.hpp>
#include <hexagon/state/connecting_state.hpp>
#include <hexagon/state/local_state.hpp>
#include <hexagon/state/world_state.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

game_facet::game_facet(int x, int y, int width, int height) noexcept
    : raw_{connecting_facet{x, y, width, height}}
{
}

connecting_facet& game_facet::get(const state::connecting_state&) noexcept
{
    return std::get<connecting_facet>(raw_);
}

world_facet& game_facet::get(const state::world_state&) noexcept
{
    return std::get<world_facet>(raw_);
}

battle_facet& game_facet::get(const state::battling_state&) noexcept
{
    return std::get<battle_facet>(raw_);
}

const connecting_facet& game_facet::get(const state::connecting_state&) const
    noexcept
{
    return std::get<connecting_facet>(raw_);
}

const world_facet& game_facet::get(const state::world_state&) const noexcept
{
    return std::get<world_facet>(raw_);
}

const battle_facet& game_facet::get(const state::battling_state&) const noexcept
{
    return std::get<battle_facet>(raw_);
}

void game_facet::resize(int w, int h) noexcept
{
    std::visit([w, h](auto& cfacet) { cfacet.resize(w, h); }, raw_);
}

void hexagon::client::draw(graphics& g, const game_facet& facet,
                           const state::local_state& s)
{
    std::visit(
        [&g, &facet](const auto& cstate) {
            hexagon::client::draw(g, facet.get(cstate), cstate);
        },
        s.raw());
}

int game_facet::width() const noexcept
{
    return std::visit([](const auto& f) { return f.width(); }, raw_);
}

int game_facet::height() const noexcept
{
    return std::visit([](const auto& f) { return f.height(); }, raw_);
}
