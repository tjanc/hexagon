//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_CONNECTING_FACET_H_
#define HEXAGON_CLIENT_CONNECTING_FACET_H_

#include <SDL.h>

namespace hexagon::model
{
    struct connecting;
}

namespace hexagon::client
{
    class graphics;
}  // namespace hexagon::client

namespace hexagon::client
{
    class connecting_facet
    {
        SDL_Rect dimensions_;

       public:
        explicit connecting_facet(int x, int y, int width, int height) noexcept;

       public:
        void resize(int w, int h) noexcept;

       public:
        int width() noexcept;
        int height() noexcept;

       public:
        void draw(graphics&, const model::connecting&) const;
    };
}  // namespace hexagon::client

#endif
