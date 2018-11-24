#ifndef HEXAGON_FONT_CACHE_H_
#define HEXAGON_FONT_CACHE_H_

#include <array>

#include "sdl/font.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon
{
    class font_cache
    {
        std::array<sdl::font, 1> fonts_;

       public:
        font_cache();

        sdl::font& at(int);

        auto begin() const { return fonts_.begin(); }
        auto end() const { return fonts_.end(); }
    };
}  // namespace hexagon

#endif
