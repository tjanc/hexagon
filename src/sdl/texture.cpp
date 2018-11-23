//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "texture.hpp"

#include "renderer.hpp"
#include "surface.hpp"

using namespace hexagon::sdl;

texture::texture(renderer& renderer, surface& surface)
    : raw_{SDL_CreateTextureFromSurface(renderer.get(), surface.get())}
{
}

void texture::set_color_mod(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    SDL_SetTextureColorMod(raw_.get(), r, g, b);
}

void texture_deleter::operator()(SDL_Texture* texture)
{
    SDL_DestroyTexture(texture);
}

texture hexagon::sdl::load_texture(renderer& renderer, const char* file)
{
    surface surface(file);
    return texture{renderer, surface};
}
