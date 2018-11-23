//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "surface.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "font.hpp"

using namespace hexagon::sdl;

namespace
{
    std::unique_ptr<SDL_Surface, surface_deleter> from_font(font& f,
                                                            const char* text,
                                                            int font_size,
                                                            SDL_Color& color)
    {
        return std::unique_ptr<SDL_Surface, surface_deleter>{
            TTF_RenderText_Solid(f.get(), text, color)};
    }
}  // namespace

surface::surface(const char* file) : raw_{IMG_Load(file)}
{
    if (!raw_) {
        std::cerr << "image not loaded"  //
                  << IMG_GetError()      //
                  << std::endl;
        return;
    }
}

surface::surface(font& f, const char* text, int font_size, SDL_Color color)
    : raw_{from_font(f, text, font_size, color)}
{
}

void surface_deleter::operator()(SDL_Surface* surface)
{
    SDL_FreeSurface(surface);
}
