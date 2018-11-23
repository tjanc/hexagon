//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_UNIT_FACET_H_
#define HEXAGON_UNIT_FACET_H_

#include <array>

#include "model/unit.hpp"
#include "sdl/texture.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon
{
    enum class perspective {
        front,
        back,
        left,
        right,
        fleft,
        fright,
        bleft,
        bright
    };

    class unit_texture_cache
    {
        std::array<sdl::texture, 1> units_;

       public:
        explicit unit_texture_cache(sdl::renderer& renderer);

        sdl::texture& at(race r, perspective p);

        auto begin() const { return units_.begin(); }
        auto end() const { return units_.end(); }
    };
}  // namespace hexagon

#endif
