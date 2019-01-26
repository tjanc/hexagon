//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "emrun.hpp"

#include <emscripten.h>

#include "connection.hpp"
#include "game.hpp"

using namespace hexagon::client;

namespace
{
    void game_loop(void *arg)
    {
        game *g = static_cast<game *>(arg);
        assert(g);
        if (g->running()) {
            if (g->handleEvents()) {
                g->update();
                g->render();
            }
        } else {
            emscripten_cancel_main_loop();
        }
    }
}  // namespace

int hexagon_emrun()
{
    game hexagon(graphics::instance(), connection::instance());

    int simulate_main_loop = 1;
    emscripten_set_main_loop_arg(game_loop, &hexagon, -1, simulate_main_loop);

    return 0;
}
