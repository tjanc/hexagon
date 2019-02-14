//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "texture.hpp"

#include "renderer.hpp"
#include "surface.hpp"
#include <SDL.h>

using namespace hexagon::sdl;

namespace
{
    SDL_Texture* createPixelizedTextureFromSurface(renderer& r, surface& s)
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        return SDL_CreateTextureFromSurface(r.get(), s.get());
    }
}  // namespace

texture::texture(renderer& r, surface& s)
    : raw_{createPixelizedTextureFromSurface(r, s)}
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

std::pair<int, int> texture::size() noexcept
{
    std::pair<int, int> result;
    SDL_QueryTexture(raw_.get(), NULL, NULL, &result.first, &result.second);
    return result;
}
