//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "battle_facet.hpp"

using namespace hexagon::client;

battle_facet::battle_facet(sdl::renderer& r) : map_facet_(r) {}

void battle_facet::draw(sdl::renderer& r, battle_controller& model)
{
    map_facet_.draw(r, model.get_map());
}

void battle_facet::mouse_move(battle_controller& model)
{
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&y, &x);

    map_facet_.mouse_over(x, y, model.get_map());
}

void battle_facet::mouse_down(battle_controller&) {}

void battle_facet::mouse_up(battle_controller& model)
{
    int x = 0;
    int y = 0;
    const auto state = SDL_GetMouseState(&y, &x);

    auto& m = model.get_map();
    map_facet_.mouse_over(x, y, m);

    auto hover = map_facet_.hover();
    if (hover != m.end() && hover->is_reachable() && hover->empty())
        model.move(hover);
}
