#include "battle_facet.hpp"

using namespace hexagon;

battle_facet::battle_facet(sdl::renderer& r) : map_facet_(r) {}

void battle_facet::draw(sdl::renderer& r, model::battle_projection& model)
{
    map_facet_.draw(r, model.get_map());
}

void battle_facet::mouse_move(model::battle_projection& model)
{
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&y, &x);

    map_facet_.mouse_over(x, y, model.get_map());
}

void battle_facet::mouse_down(model::battle_projection&) {}

void battle_facet::mouse_up(model::battle_projection& model)
{
    int x = 0;
    int y = 0;
    const auto state = SDL_GetMouseState(&y, &x);

    model::map& m = model.get_map();
    map_facet_.mouse_over(x, y, m);

    auto hover = map_facet_.hover();
    if (hover != m.end() && hover->is_reachable()) model.move(hover);
}
