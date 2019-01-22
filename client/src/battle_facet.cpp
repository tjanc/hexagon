//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_facet.hpp"

#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>
#include <hexagon/state/battling_state.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    constexpr int clamp_0(int v) { return v < 0 ? 0 : v; }
    constexpr int clamp_top(int v, int c) { return v > c ? c : v; }
}  // namespace

battle_facet::battle_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height},
      map_facet_{x, y, width, height}
{
}

const map_facet& battle_facet::map() const noexcept { return map_facet_; }
map_facet& battle_facet::map() noexcept { return map_facet_; }

int battle_facet::width() const noexcept { return dimensions_.w; }
int battle_facet::height() const noexcept { return dimensions_.h; }

void battle_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
    map_facet_.resize(w, h);
}

basic_map_index hexagon::client::transpose(const battle_facet& facet,
                                           const state::battling_state& state,
                                           int x, int y) noexcept
{
    return transpose(facet.map(), state, x, y);
}

void hexagon::client::draw(graphics& g, const battle_facet& facet,
                           const state::battling_state& s)
{
    draw(g, facet.map(), s);
}
