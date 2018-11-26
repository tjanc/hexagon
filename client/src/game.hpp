//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GAME_H_
#define HEXAGON_CLIENT_GAME_H_

#include "battle_facet.hpp"
#include "mouse.hpp"

#include "sdl/renderer.hpp"
#include "sdl/sdl.hpp"
#include "sdl/window.hpp"

#include "game_controller.hpp"

namespace hexagon::client
{
    class game
    {
        // graphics
        sdl::sdl graphics_;
        mouse mouse_;

        sdl::window window_;
        sdl::renderer renderer_;

        // models
        game_controller game_controller_;

        // screens
        battle_facet battle_;

        // running?
        bool running_ = true;

       public:
        game(int x, int y, int width, int height, bool fullscreen);

       public:
        bool handleEvents();
        void update();
        void render();
        void clean();
        bool running() const { return running_; }
    };
}  // namespace hexagon::client

#endif
