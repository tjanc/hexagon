//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_facet.hpp"

#include <hexagon/model/map.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>
#include <iostream>

#include "battle_controller.hpp"
#include "graphics.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    constexpr int TILE_WIDTH = 37;
    constexpr int TILE_HEIGHT = 26;

    constexpr int TOP_VERTEX_X_OFFSET = TILE_WIDTH / 2;
    constexpr int TOP_LEFT_Y_VERTEX_OFFSET = 8;
    constexpr int BOTTOM_LEFT_Y_VERTEX_OFFSET =
        TILE_HEIGHT - TOP_LEFT_Y_VERTEX_OFFSET;

    constexpr int BY_ROW_X_OFFSET = TOP_VERTEX_X_OFFSET;

    constexpr int ROW_HEIGHT = BOTTOM_LEFT_Y_VERTEX_OFFSET + 1;
    constexpr int COLUMN_WIDTH = TILE_WIDTH;

    constexpr int BOX_HEIGHT = 8;

    constexpr int UNIT_HEIGHT = 62;
    constexpr int UNIT_WIDTH = 72;

}  // namespace

map_facet::map_facet(int x, int y, int width, int height) noexcept
    : dimensions_{.x = x, .y = y, .w = width, .h = height}, hover_tile_{}
{
}

basic_map_index map_facet::transpose(int x, int y) const noexcept
{
    const int y_t = y / ROW_HEIGHT;
    const int x_t = (x - (y_t % 2) * (COLUMN_WIDTH / 2)) / COLUMN_WIDTH;

    return basic_map_index{static_cast<std::uint32_t>(x_t),
                           static_cast<std::uint32_t>(y_t)};
}

namespace
{
    void render_elevation(graphics& renderer, const tile& t, int x, int y)
    {
        auto& filler = renderer.tiles().tile_filler(t.type());
        const auto fsize = filler.size();
        SDL_Rect destination = {
            .x = x, .y = y, .w = fsize.first, .h = fsize.second};

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
                .y = destination.y - usize.second + TILE_HEIGHT,
                .w = usize.first,
                .h = usize.second};

            renderer->copy(unit_texture, unit_dest);
        }
    }
}  // namespace

void map_facet::draw(graphics& renderer, const map& m,
                     const units_moved& model) const
{
    iterate(m, [&renderer, this](const auto& t, auto idx) {
        auto tile_x = tile_base_x(dimensions_, idx.x, idx.y);
        auto tile_y = tile_base_y(dimensions_, idx.y);

        render_elevation(renderer, t, tile_x, tile_y);

        const int elev = t.elevation();
        SDL_Rect destination = {.x = tile_x,
                                .y = tile_y - ROW_HEIGHT - elev * BOX_HEIGHT,
                                .w = TILE_WIDTH,
                                .h = TILE_HEIGHT};

        // tile surface
        {
            sdl::texture& tile_texture =
                renderer.tiles().tile_surface(t.type());
            if (hover_tile_ == idx)
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
    renderer->fill_rect(dimensions_);
}

void map_facet::draw(graphics& renderer, const map& m,
                     const unit_moving& model) const
{
    renderer->set_draw_blend_mode(SDL_BLENDMODE_ADD);

    iterate(m, [&m, &model, &renderer, this](const auto& t, auto idx) {
        auto tile_x = tile_base_x(dimensions_, idx.x, idx.y);
        auto tile_y = tile_base_y(dimensions_, idx.y);

        render_elevation(renderer, t, tile_x, tile_y);

        const int elev = t.elevation();
        SDL_Rect destination = {.x = tile_x,
                                .y = tile_y - ROW_HEIGHT - elev * BOX_HEIGHT,
                                .w = TILE_WIDTH,
                                .h = TILE_HEIGHT};

        // tile surface
        {
            sdl::texture& tile_texture =
                renderer.tiles().tile_surface(t.type());

            if (hover_tile_ == idx)
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

void map_facet::hover(basic_map_index t) noexcept { hover_tile_ = t; }

basic_map_index map_facet::hover() const noexcept { return hover_tile_; }

void map_facet::resize(int w, int h) noexcept
{
    dimensions_.w = w;
    dimensions_.h = h;
}
