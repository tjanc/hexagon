//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_TEXT_H_
#define HEXAGON_SDL_TEXT_H_

#include "texture.hpp"

namespace hexagon::sdl
{
    class renderer;
    class font;
}  // namespace hexagon::sdl

namespace hexagon::sdl
{
    texture load_text(renderer&, font&, const char*, int, SDL_Color&);
}  // namespace hexagon::sdl

#endif
