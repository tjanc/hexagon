//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "sdl.hpp"

#include <SDL.h>
#include <SDL_hints.h>
#include <iostream>

using namespace hexagon::sdl;

sdl::sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "unable to initialize SDL, aborting";
        abort();
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

sdl::~sdl() { SDL_Quit(); }
