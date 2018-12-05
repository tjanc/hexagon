//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GAME_H_
#define HEXAGON_CLIENT_GAME_H_

#include "battle_facet.hpp"
#include "mouse.hpp"

#include "sdl/sdl.hpp"
#include "sdl/window.hpp"

#include "canvas.hpp"
#include "game_controller.hpp"

namespace hexagon::client
{
    class connection;

    class game
    {
        connection& server_;
        // graphics
        sdl::sdl graphics_;
        mouse mouse_;

        sdl::window window_;
        canvas canvas_;

        // models
        game_controller game_controller_;

        // running?
        bool running_ = true;

       public:
        game(connection& c, int x, int y, int width, int height,
             bool fullscreen);

       public:
        bool handleEvents();
        void update();
        void render();
        void clean();
        bool running() const { return running_; }
    };
}  // namespace hexagon::client

#endif
