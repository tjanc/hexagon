//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SDL_SDL_H_
#define HEXAGON_SDL_SDL_H_

#include <string>

namespace hexagon::sdl
{
    struct exception
    {
        std::string what;
    };

    class sdl
    {
       public:
        sdl();
        sdl(const sdl&) = delete;
        sdl(sdl&&) = delete;
        sdl& operator=(const sdl&) = delete;
        sdl& operator=(sdl&&) = delete;
        ~sdl();
    };

}  // namespace hexagon::sdl

#endif
