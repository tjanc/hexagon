//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_UNIT_FACET_H_
#define HEXAGON_CLIENT_UNIT_FACET_H_

#include <array>

#include <hexagon/model/unit.hpp>
#include "sdl/texture.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon::client
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
        std::array<hexagon::sdl::texture, 1> units_;

       public:
        explicit unit_texture_cache(hexagon::sdl::renderer& renderer);

        sdl::texture& at(model::race r, perspective p);

        auto begin() const { return units_.begin(); }
        auto end() const { return units_.end(); }
    };
}  // namespace hexagon::client

#endif
