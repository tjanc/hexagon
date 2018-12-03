//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_facet.hpp"

#include <hexagon/model/map.hpp>
#include <hexagon/model/map_loader.hpp>

#include "sdl/renderer.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    constexpr int TILE_WIDTH = 37;
    constexpr int TILE_HEIGHT = 26;

    constexpr int TOP_VERTEX_X_OFFSET = TILE_WIDTH / 2;
    constexpr int TOP_LEFT_Y_VERTEX_OFFSET = 8;
    constexpr int BOTTOM_LEFT_Y_VERTEX_OFFSET = TILE_HEIGHT - 8;

    constexpr int BY_ROW_X_OFFSET = TOP_VERTEX_X_OFFSET;

    constexpr int ROW_HEIGHT = BOTTOM_LEFT_Y_VERTEX_OFFSET;
    constexpr int COLUMN_WIDTH = TILE_WIDTH;

    constexpr int BOX_HEIGHT = 8;

    constexpr int UNIT_HEIGHT = 62;
    constexpr int UNIT_WIDTH = 72;

}  // namespace

map_facet::map_facet(sdl::renderer& renderer)
    : tile_textures_(renderer), unit_textures_(renderer), hover_tile_{}
{
}

void map_facet::mouse_over(int x, int y, map& m)
{
    const int y_hover = x / ROW_HEIGHT;
    const int x_hover = (y - (y_hover % 2) * (COLUMN_WIDTH / 2)) / COLUMN_WIDTH;

    hover_tile_ = m.find(x_hover, y_hover);
}

void map_facet::draw(sdl::renderer& renderer, const map& model)
{
    renderer.set_draw_blend_mode(SDL_BLENDMODE_ADD);

    int n = 0;
    for (auto it = model.begin(); it != model.end(); ++it, ++n) {
        const int i = n % model.width();
        const int j = n / model.width();

        const int elev = it->elevation();

        SDL_Rect destination = {
            .x = i * COLUMN_WIDTH + (j % 2) * BY_ROW_X_OFFSET,
            .y = j * ROW_HEIGHT + TILE_HEIGHT - 15,
            .w = TILE_WIDTH,
            .h = 15};
        // elevation filler
        {
            sdl::texture& filler = tile_textures_.tile_filler(it->type());
            for (int e = 0; e <= elev; ++e) {
                const uint8_t c = e < 3 ? (170 + e * 20) : 230;
                filler.set_color_mod(c, c, c);

                destination.y -= BOX_HEIGHT;
                renderer.copy(filler, destination);
            }
        }

        destination.h = TILE_HEIGHT;
        destination.y -= TILE_HEIGHT - 15;

        // tile surface
        {
            sdl::texture& tile_texture =
                tile_textures_.tile_surface(it->type());
            if (hover_tile_ == it)
                tile_texture.set_color_mod(255, 255, 255);
            else {
                if (it->is_reachable() && it->empty()) {
                    tile_texture.set_color_mod(200, 200, 255);
                } else {
                    const uint8_t c = elev < 3 ? (170 + elev * 20) : 230;
                    tile_texture.set_color_mod(c, c, c);
                }
            }
            renderer.copy(tile_texture, destination);
        }

        // object
        if (const unit* u = it->get_if_unit()) {
            sdl::texture& unit_texture =
                unit_textures_.at(u->race_, perspective::fright);
            destination.y -= BOX_HEIGHT + BOX_HEIGHT / 2;
            SDL_Rect unit_dest = {
                .x = destination.x - (UNIT_WIDTH - TILE_WIDTH) / 2,
                .y = destination.y - (UNIT_HEIGHT - TILE_HEIGHT) / 2,
                .w = UNIT_WIDTH,
                .h = UNIT_HEIGHT};

            renderer.copy(unit_texture, unit_dest);
        }
    }
}
