//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_facet.hpp"

#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>

using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    constexpr int MAP_MARGIN = 20;

    constexpr int clamp_0(int v) { return v < 0 ? 0 : v; }
    constexpr int clamp_top(int v, int c) { return v > c ? c : v; }

    constexpr int map_x(int x, int width)
    {
        return clamp_top(x + MAP_MARGIN, x + width);
    }
    constexpr int map_y(int y, int height)
    {
        return clamp_top(y + MAP_MARGIN, y + height);
    }

    constexpr int map_width(int width)
    {
        return clamp_0(width - 2 * MAP_MARGIN);
    }

    constexpr int map_height(int height)
    {
        return clamp_0(height - 2 * MAP_MARGIN);
    }

}  // namespace

battle_facet::battle_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height},
      map_facet_{map_x(x, width), map_y(y, height), map_width(width),
                 map_height(height)}
{
}

void battle_facet::draw(graphics& c, const model::map& m,
                        const unit_moving& model) const
{
    map_facet_.draw(c, m, model);
}

void battle_facet::draw(graphics& c, const model::map& m,
                        const units_moved& model) const
{
    map_facet_.draw(c, m, model);
}

const map_facet& battle_facet::map() const noexcept { return map_facet_; }
map_facet& battle_facet::map() noexcept { return map_facet_; }

int battle_facet::width() const noexcept { return dimensions_.w; }
int battle_facet::height() const noexcept { return dimensions_.h; }

void battle_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
    map_facet_.resize(map_width(w), map_height(h));
}

basic_map_index battle_facet::transpose(int x, int y) const noexcept
{
    return map_facet_.transpose(x - MAP_MARGIN, y - MAP_MARGIN);
    // return map_facet_.transpose(x, y);
}
