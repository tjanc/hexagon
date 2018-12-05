//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "renderer.hpp"

#include <iostream>
#include "texture.hpp"
#include "window.hpp"

using namespace hexagon::sdl;

renderer::renderer(window& window)
    : raw_{SDL_CreateRenderer(window.get(), -1, 0)}
{
    if (!raw_) throw exception{"unable to create renderer"};
}

void renderer::set_draw_blend_mode(SDL_BlendMode mode)
{
    if (0 != SDL_SetRenderDrawBlendMode(raw_.get(), mode))
        std::cerr << "additive blending not supported";
};

void renderer::copy(texture& texture, const SDL_Rect& destination)
{
    SDL_RenderCopy(raw_.get(), texture.get(), NULL, &destination);
}

void renderer::present() { SDL_RenderPresent(raw_.get()); }

void renderer::clear() { SDL_RenderClear(raw_.get()); }

void renderer::set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b,
                              std::uint8_t a)
{
    SDL_SetRenderDrawColor(raw_.get(), r, g, b, a);
}

void renderer_deleter::operator()(SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
}
