#include "map_facet.hpp"

#include <hexagon/model/map.hpp>
#include <hexagon/model/map_loader.hpp>

#include "sdl/renderer.hpp"

using namespace hexagon;

namespace
{
    constexpr int TILE_WIDTH = 56;
    constexpr int TILE_HEIGHT = 56;

    constexpr int TOP_VERTEX_X_OFFSET = TILE_WIDTH / 2;
    constexpr int TOP_LEFT_Y_VERTEX_OFFSET = 12;
    constexpr int BOTTOM_LEFT_Y_VERTEX_OFFSET = 33;

    constexpr int BY_ROW_X_OFFSET = TOP_VERTEX_X_OFFSET;

    constexpr int ROW_HEIGHT = BOTTOM_LEFT_Y_VERTEX_OFFSET;
    constexpr int COLUMN_WIDTH = TILE_WIDTH;

    constexpr int BOX_HEIGHT = 11;

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
            .y = j * ROW_HEIGHT,
            .w = TILE_WIDTH,
            .h = TILE_HEIGHT};

        // elevation filler
        {
            sdl::texture& filler = tile_textures_.tile_filler(it->type());
            for (int e = 0; e < elev; ++e) {
                const uint8_t c = e < 3 ? (170 + e * 20) : 230;
                filler.set_color_mod(c, c, c);

                destination.y -= BOX_HEIGHT;
                renderer.copy(filler, destination);
            }
        }

        // tile surface
        {
            sdl::texture& tile_texture =
                tile_textures_.tile_surface(it->type());
            if (hover_tile_ == it)
                tile_texture.set_color_mod(255, 255, 255);
            else {
                if (it->is_reachable()) {
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
            renderer.copy(unit_texture, destination);
        }
    }
}
