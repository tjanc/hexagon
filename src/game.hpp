#ifndef HEXAGON_GAME_H_
#define HEXAGON_GAME_H_

#include "battle_facet.hpp"
#include "mouse.hpp"

#include "sdl/renderer.hpp"
#include "sdl/sdl.hpp"
#include "sdl/window.hpp"

#include "model/game_projection.hpp"

namespace hexagon
{
    class game
    {
        // graphics
        sdl::sdl graphics_;
        mouse mouse_;

        sdl::window window_;
        sdl::renderer renderer_;

        // proxys
        model::battle battle_model_;  // TODO should be proxy to server

        // models
        model::game_projection game_projection_;

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
}  // namespace hexagon

#endif
