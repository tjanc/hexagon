//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_facet.hpp"

#include <hexagon/model/map.hpp>
#include <hexagon/model/unit_moved.hpp>
#include <hexagon/model/unit_moving.hpp>

#include "battle_controller.hpp"
#include "canvas.hpp"
#include "moving_controller.hpp"

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

    constexpr int ROW_HEIGHT = BOTTOM_LEFT_Y_VERTEX_OFFSET;
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

void map_facet::draw(canvas& renderer, const unit_moved& model) const
{
    const auto& m = model.battlefield().get_map();
    iterate(m, [&model, &renderer, this](const auto& t, auto idx) {
        const int elev = t.elevation();

        const int x = idx.x;
        const int y = idx.y;

        SDL_Rect destination = {
            .x = x * COLUMN_WIDTH + (y % 2) * BY_ROW_X_OFFSET,
            .y = y * ROW_HEIGHT + TILE_HEIGHT - 15,
            .w = TILE_WIDTH,
            .h = 15};
        // elevation filler
        {
            sdl::texture& filler = renderer.tiles().tile_filler(t.type());
            for (int e = 0; e <= elev; ++e) {
                const uint8_t c = e < 3 ? (170 + e * 20) : 230;
                filler.set_color_mod(c, c, c);

                destination.y -= BOX_HEIGHT;
                renderer->copy(filler, destination);
            }
        }

        destination.h = TILE_HEIGHT;
        destination.y -= TILE_HEIGHT - 15;

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

        // object
        if (const unit* u = t.get_if_unit()) {
            auto& unit_texture =
                renderer.units().at(u->job(), perspective::fright);
            destination.y -= BOX_HEIGHT + BOX_HEIGHT / 2;
            SDL_Rect unit_dest = {
                .x = destination.x - (UNIT_WIDTH - TILE_WIDTH) / 2,
                .y = destination.y - (UNIT_HEIGHT - TILE_HEIGHT) / 2,
                .w = UNIT_WIDTH,
                .h = UNIT_HEIGHT};

            renderer->copy(unit_texture, unit_dest);
        }
    });

    renderer->set_draw_color(0, 0, 0, 40);
    renderer->fill_rect(dimensions_);
}

void map_facet::draw(canvas& renderer, const unit_moving& model) const
{
    renderer->set_draw_blend_mode(SDL_BLENDMODE_ADD);

    const auto& m = model.battlefield().get_map();
    iterate(m, [&model, &renderer, this](const auto& t, auto idx) {
        const int elev = t.elevation();

        const int x = idx.x;
        const int y = idx.y;

        SDL_Rect destination = {
            .x = x * COLUMN_WIDTH + (y % 2) * BY_ROW_X_OFFSET,
            .y = y * ROW_HEIGHT + TILE_HEIGHT - 15,
            .w = TILE_WIDTH,
            .h = 15};
        // elevation filler
        {
            sdl::texture& filler = renderer.tiles().tile_filler(t.type());
            for (int e = 0; e <= elev; ++e) {
                const uint8_t c = e < 3 ? (170 + e * 20) : 230;
                filler.set_color_mod(c, c, c);

                destination.y -= BOX_HEIGHT;
                renderer->copy(filler, destination);
            }
        }

        destination.h = TILE_HEIGHT;
        destination.y -= TILE_HEIGHT - 15;

        // tile surface
        {
            sdl::texture& tile_texture =
                renderer.tiles().tile_surface(t.type());
            if (hover_tile_ == idx)
                tile_texture.set_color_mod(255, 255, 255);
            else {
                if (model.reachable(idx)) {
                    tile_texture.set_color_mod(200, 200, 255);
                } else {
                    const uint8_t c = elev < 3 ? (170 + elev * 20) : 230;
                    tile_texture.set_color_mod(c, c, c);
                }
            }
            renderer->copy(tile_texture, destination);
        }

        // object
        if (const unit* u = t.get_if_unit()) {
            auto& unit_texture =
                renderer.units().at(u->job(), perspective::fright);
            destination.y -= BOX_HEIGHT + BOX_HEIGHT / 2;
            SDL_Rect unit_dest = {
                .x = destination.x - (UNIT_WIDTH - TILE_WIDTH) / 2,
                .y = destination.y - (UNIT_HEIGHT - TILE_HEIGHT) / 2,
                .w = UNIT_WIDTH,
                .h = UNIT_HEIGHT};

            renderer->copy(unit_texture, unit_dest);
        }
    });
}

void map_facet::hover(basic_map_index t) noexcept { hover_tile_ = t; }

basic_map_index map_facet::hover() const noexcept { return hover_tile_; }
