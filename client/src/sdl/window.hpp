//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_WINDOW_H_
#define HEXAGON_SDL_WINDOW_H_

#include <SDL.h>
#include <memory>
#include "sdl.hpp"

namespace hexagon::sdl
{
    struct window_deleter {
        void operator()(SDL_Window*);
    };

    class window
    {
        std::unique_ptr<SDL_Window, window_deleter> raw_;

       public:
        window(sdl& graphics, const char* title, int x, int y, int width,
               int height, bool fullscreen = false);
        window(sdl&, const char* title, int width, int height);

       public:
        SDL_Window* get() { return raw_.get(); }
        const SDL_Window* get() const { return raw_.get(); }

       public:
        SDL_Rect size() const;

       public:
        void resize(int w, int h) const;

       public:
        explicit operator bool() const { return nullptr != raw_.get(); }
    };

}  // namespace hexagon::sdl

#endif
