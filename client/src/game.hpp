//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GAME_H_
#define HEXAGON_CLIENT_GAME_H_

#include <hexagon/state/local_state.hpp>

#include "game_facet.hpp"
#include "mouse.hpp"

#include "sdl/sdl.hpp"
#include "sdl/window.hpp"

#include "graphics.hpp"

namespace hexagon::client
{
    class connection;

    class game
    {
        graphics& graphics_;
        connection& server_;

        mouse mouse_;

        state::local_state state_;
        game_facet facet_;

        bool running_ = true;

       public:
        game(graphics& g, connection& c);

       public:
        bool handleEvents();
        void update();
        void render();
        void clean();
        bool running() const { return running_; }
    };
}  // namespace hexagon::client

#endif
