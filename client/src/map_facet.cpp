//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_facet.hpp"

#include <algorithm>
#include <hexagon/model/map.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>
#include <hexagon/state/battling_state.hpp>
#include <iostream>

#include "graphics.hpp"

using namespace hexagon;
using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    constexpr int TILE_WIDTH = 37 * 2;
    constexpr int TILE_HEIGHT = 26 * 2;
    constexpr int TOP_LEFT_Y_VERTEX_OFFSET = 8 * 2;
    constexpr int BOX_HEIGHT = 8 * 2;
    constexpr int UNIT_HEIGHT = 62 * 2;
    constexpr int UNIT_WIDTH = 72 * 2;

    constexpr int TOP_VERTEX_X_OFFSET = TILE_WIDTH / 2;
    constexpr int BOTTOM_LEFT_Y_VERTEX_OFFSET =
        TILE_HEIGHT - TOP_LEFT_Y_VERTEX_OFFSET;

    constexpr int BY_ROW_X_OFFSET = TOP_VERTEX_X_OFFSET;

    constexpr int ROW_HEIGHT = BOTTOM_LEFT_Y_VERTEX_OFFSET + 1;
    constexpr int COLUMN_WIDTH = TILE_WIDTH;

}  // namespace

map_facet::map_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height}, hover_tile_{}
{
}

namespace
{
    void render_elevation(graphics& renderer, const tile& t, int x, int y)
    {
        auto& filler = renderer.tiles().tile_filler(t.type());
        const auto fsize = filler.size();
        SDL_Rect destination = {
            .x = x, .y = y, .w = fsize.first * 2, .h = fsize.second * 2};

        for (int e = 0; e < t.elevation(); ++e) {
            const uint8_t c = e < 3 ? (170 + e * 20) : 230;
            filler.set_color_mod(c, c, c);

            destination.y -= BOX_HEIGHT;
            renderer->copy(filler, destination);
        }
    }

    constexpr int tile_base_x(const SDL_Rect& dim, int x, int y)
    {
        return dim.x + x * COLUMN_WIDTH + (y % 2) * BY_ROW_X_OFFSET;
    }

    constexpr int tile_base_y(const SDL_Rect& dim, int y)
    {
        return ROW_HEIGHT + dim.y + y * ROW_HEIGHT + TILE_HEIGHT - 15;
    }

    SDL_Rect tile_base_destination(const SDL_Rect& dim, int x, int y)
    {
        return SDL_Rect{.x = tile_base_x(dim, x, y),
                        .y = tile_base_y(dim, y),
                        .w = TILE_WIDTH,
                        .h = 15};
    }

    void render_objects(graphics& renderer, const tile& t, SDL_Rect destination)
    {
        if (const unit* u = t.get_if_unit()) {
            auto& unit_texture =
                renderer.units().at(u->job(), perspective::fright);
            const auto usize = unit_texture.size();
            SDL_Rect unit_dest = {
                .x = destination.x,
                .y = destination.y - usize.second * 2 + TILE_HEIGHT,
                .w = usize.first * 2,
                .h = usize.second * 2};

            renderer->copy(unit_texture, unit_dest);
        }
    }
}  // namespace

namespace
{
    void draw_specific(graphics& renderer, const map_facet& facet, const map& m,
                       const units_moved& model)
    {
        iterate(m, [&renderer, &facet](const auto& t, auto idx) {
            auto tile_x = tile_base_x(facet.dimensions(), idx.x, idx.y);
            auto tile_y = tile_base_y(facet.dimensions(), idx.y);

            render_elevation(renderer, t, tile_x, tile_y);

            const int elev = t.elevation();
            SDL_Rect destination = {
                .x = tile_x,
                .y = tile_y - ROW_HEIGHT - elev * BOX_HEIGHT,
                .w = TILE_WIDTH,
                .h = TILE_HEIGHT};

            // tile surface
            {
                sdl::texture& tile_texture =
                    renderer.tiles().tile_surface(t.type());
                if (facet.hover() == idx)
                    tile_texture.set_color_mod(255, 255, 255);
                else {
                    const uint8_t c = elev < 3 ? (170 + elev * 20) : 230;
                    tile_texture.set_color_mod(c, c, c);
                }
                renderer->copy(tile_texture, destination);
            }

            render_objects(renderer, t, destination);
        });

        renderer->set_draw_color(30, 30, 30, 255);
        renderer->fill_rect(facet.dimensions());
    }

    std::pair<int, int> tile_base(const map_facet& facet,
                                  const unit_moving& model, basic_map_index idx)
    {
        const auto unit_pos = model.position();
        const auto facet_size = facet.dimensions();

        const auto x_delta = idx.x - unit_pos.x;
        const auto y_delta = idx.y - unit_pos.y;

        const auto tile_x =
            facet_size.w / 2 - (COLUMN_WIDTH / 2) + x_delta * COLUMN_WIDTH +
            (idx.y % 2) * BY_ROW_X_OFFSET - (unit_pos.y % 2) * BY_ROW_X_OFFSET;
        const auto tile_y = facet_size.h / 2 + y_delta * ROW_HEIGHT;

        return std::pair{tile_x, tile_y};
    }

    void draw_specific(graphics& renderer, const map_facet& facet, const map& m,
                       const unit_moving& model)
    {
        renderer->set_draw_blend_mode(SDL_BLENDMODE_ADD);

        iterate(m, [&m, &model, &renderer, &facet](const auto& t, auto idx) {
            auto [tile_x, tile_y] = tile_base(facet, model, idx);

            render_elevation(renderer, t, tile_x, tile_y);

            const int elev = t.elevation();
            SDL_Rect destination = {
                .x = tile_x,
                .y = tile_y - ROW_HEIGHT - elev * BOX_HEIGHT,
                .w = TILE_WIDTH,
                .h = TILE_HEIGHT};

            // tile surface
            {
                sdl::texture& tile_texture =
                    renderer.tiles().tile_surface(t.type());

                if (facet.hover() == idx)
                    tile_texture.set_color_mod(255, 255, 255);
                else {
                    const uint8_t c = elev < 3 ? (170 + elev * 20) : 230;
                    tile_texture.set_color_mod(c, c, c);
                }
                renderer->copy(tile_texture, destination);
            }

            if (model.reachable(m, idx))
                renderer->copy(renderer.tiles().tile_hover(), destination);

            render_objects(renderer, t, destination);
        });
    }
}  // namespace

void map_facet::hover(basic_map_index t) noexcept { hover_tile_ = t; }

basic_map_index map_facet::hover() const noexcept { return hover_tile_; }

void map_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
}

const SDL_Rect& map_facet::dimensions() const noexcept { return dimensions_; }

void hexagon::client::draw(graphics& g, const map_facet& facet,
                           const state::battling_state& s)
{
    std::visit(
        [&g, &facet, &m = s.get_battle().get_map()](const auto& cstate) {  //
            draw_specific(g, facet, m, cstate);
        },
        s.raw());
}

namespace
{
    basic_map_index transpose_specific(const map_facet& facet,
                                       const unit_moving& model, int x,
                                       int y) noexcept
    {
        const auto& unit_pos = model.position();
        const auto& facet_size = facet.dimensions();

        const int mid_y = facet_size.h / 2 - ROW_HEIGHT / 2;
        const int mid_x = facet_size.w / 2 - COLUMN_WIDTH / 2;

        const int delta_y = y - mid_y;
        const int y_idx =
            unit_pos.y + delta_y / ROW_HEIGHT + (delta_y < 0 ? -1 : 0);

        const int delta_x =
            x - mid_x +
            ((std::llabs(y_idx - static_cast<int>(unit_pos.y)) % 2) *
             (-BY_ROW_X_OFFSET + (unit_pos.y % 2) * COLUMN_WIDTH));
        const int x_idx =
            unit_pos.x + delta_x / COLUMN_WIDTH + (delta_x < 0 ? -1 : 0);

        return basic_map_index{static_cast<std::uint32_t>(x_idx),
                               static_cast<std::uint32_t>(y_idx)};
    }

    basic_map_index transpose_specific(const map_facet& facet,
                                       const units_moved& model, int x,
                                       int y) noexcept
    {
        const int y_t = y / ROW_HEIGHT;
        const int x_t = (x - (y_t % 2) * (COLUMN_WIDTH / 2)) / COLUMN_WIDTH;

        return basic_map_index{static_cast<std::uint32_t>(x_t),
                               static_cast<std::uint32_t>(y_t)};
    }
}  // namespace

basic_map_index hexagon::client::transpose(const map_facet& facet,
                                           const state::battling_state& s,
                                           int x, int y) noexcept
{
    return std::visit(
        [&facet, x, y](const auto& model) {
            return transpose_specific(facet, model, x, y);
        },
        s.raw());
}
