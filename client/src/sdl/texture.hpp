//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_TEXTURE_H_
#define HEXAGON_SDL_TEXTURE_H_

#include <SDL.h>
#include <memory>
#include <utility>

namespace hexagon::sdl
{
    class renderer;
    class surface;
}  // namespace hexagon::sdl

namespace hexagon::sdl
{
    struct texture_deleter {
        void operator()(SDL_Texture* texture);
    };

    class texture
    {
        std::unique_ptr<SDL_Texture, texture_deleter> raw_;

       public:
        texture(renderer& renderer, surface& surface);

       public:
        SDL_Texture* get() { return raw_.get(); }
        const SDL_Texture* get() const { return raw_.get(); }

       public:
        explicit operator bool() const { return nullptr != raw_.get(); }

       public:
        void set_color_mod(std::uint8_t r, std::uint8_t g, std::uint8_t b);

       public:
        std::pair<int, int> size() noexcept;
    };

    texture load_texture(renderer& renderer, const char* file);

}  // namespace hexagon::sdl

#endif
