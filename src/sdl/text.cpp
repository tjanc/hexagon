//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "text.hpp"

#include "surface.hpp"

using namespace hexagon::sdl;

texture hexagon::sdl::load_text(renderer& renderer, font& f, const char* text,
                                int size, SDL_Color& color)
{
    surface s(f, text, size, color);
    return texture{renderer, s};
}
