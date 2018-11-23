//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_FONT_H_
#define HEXAGON_SDL_FONT_H_

#include <SDL2/SDL_ttf.h>
#include <memory>

namespace hexagon::sdl
{
    struct font_deleter {
        void operator()(TTF_Font* texture);
    };

    class font
    {
        std::unique_ptr<TTF_Font, font_deleter> raw_;

       public:
        font(const char* file, int size);

       public:
        TTF_Font* get() { return raw_.get(); }
        const TTF_Font* get() const { return raw_.get(); }

       public:
        explicit operator bool() const { return nullptr != raw_.get(); }
    };
}  // namespace hexagon::sdl

#endif
