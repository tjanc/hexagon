//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_RENDERER_H_
#define HEXAGON_SDL_RENDERER_H_

#include <SDL.h>
#include <memory>

namespace hexagon::sdl
{
    class window;
    class texture;

}  // namespace hexagon::sdl

namespace hexagon::sdl
{
    class window;

    struct renderer_deleter {
        void operator()(SDL_Renderer*);
    };

    class renderer
    {
        std::unique_ptr<SDL_Renderer, renderer_deleter> raw_;

       public:
        explicit renderer(window&);

       public:
        SDL_Renderer* get() { return raw_.get(); }
        const SDL_Renderer* get() const { return raw_.get(); }

       public:
        explicit operator bool() const { return nullptr != raw_.get(); }

       public:
        void set_draw_blend_mode(SDL_BlendMode mode);
        void set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b,
                            std::uint8_t a);
        void copy(texture& texture, const SDL_Rect& desination);
        void fill_rect(const SDL_Rect& rect);

       public:
        void present();
        void clear();
    };

}  // namespace hexagon::sdl

#endif
