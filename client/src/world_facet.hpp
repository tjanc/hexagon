//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_WORLD_FACET_H_
#define HEXAGON_CLIENT_WORLD_FACET_H_

#include <SDL.h>

namespace hexagon::model
{
    struct world;
}

namespace hexagon::client
{
    class canvas;
}  // namespace hexagon::client

namespace hexagon::client
{
    class world_facet
    {
        SDL_Rect dimensions_;

       public:
        explicit world_facet(int x, int y, int width, int height) noexcept;

       public:
        void draw(canvas&, const model::world&) const;
    };
}  // namespace hexagon::client

#endif
