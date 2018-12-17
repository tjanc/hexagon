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

window::window(sdl&, const char* title, int width, int height)
    : raw_{SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_SHOWN)}
{
    if (!raw_) throw exception{"unable to create window"};
}

SDL_Rect window::size() const
{
    SDL_Rect dim{0, 0, 0, 0};
    SDL_GetWindowSize(raw_.get(), &dim.w, &dim.h);
    return dim;
}

void window::resize(int w, int h) const
{
    SDL_SetWindowSize(raw_.get(), w, h);
}

void window_deleter::operator()(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}
