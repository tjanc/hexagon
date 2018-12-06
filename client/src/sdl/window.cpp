//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "window.hpp"

#include <SDL.h>
#include "sdl.hpp"

using namespace hexagon::sdl;

window::window(sdl&, const char* title, int x, int y, int width, int height,
               bool fullscreen)
    : raw_{SDL_CreateWindow(title, x, y, width, height,
                            fullscreen ? SDL_WINDOW_FULLSCREEN : 0)}
{
    if (!raw_) throw exception{"unable to create window"};
}

SDL_Rect window::size() const
{
    SDL_Rect dim{0, 0, 0, 0};
    SDL_GetWindowSize(raw_.get(), &dim.w, &dim.h);
    return dim;
}

void window_deleter::operator()(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}
