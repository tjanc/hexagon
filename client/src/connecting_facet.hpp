//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_CONNECTING_FACET_H_
#define HEXAGON_CLIENT_CONNECTING_FACET_H_

#include <SDL.h>

namespace hexagon::state
{
    class connecting_state;
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
        connecting_facet(int x, int y, int width, int height) noexcept;

       public:
        void resize(int w, int h) noexcept;

       public:
        int width() const noexcept;
        int height() const noexcept;
    };

    void draw(graphics&, const connecting_facet&,
              const state::connecting_state&);
}  // namespace hexagon::client

#endif
