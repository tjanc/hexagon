//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_SURFACE_H_
#define HEXAGON_SDL_SURFACE_H_

#include <SDL.h>
#include <memory>

namespace hexagon::sdl
{
    class font;
}

namespace hexagon::sdl
{
    struct surface_deleter {
        void operator()(SDL_Surface*);
    };

    class surface
    {
        std::unique_ptr<SDL_Surface, surface_deleter> raw_;

       public:
        explicit surface(const char* image_file);
        surface(font& f, const char* text, int font_size,
                SDL_Color color);

       public:
        SDL_Surface* get() { return raw_.get(); }
        const SDL_Surface* get() const { return raw_.get(); }

       public:
        explicit operator bool() const { return nullptr != raw_.get(); }
    };

}  // namespace hexagon::sdl

#endif
