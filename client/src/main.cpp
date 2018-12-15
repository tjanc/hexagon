//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <emscripten.h>

#include "game.hpp"
#include "connection.hpp"

using namespace hexagon::client;

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

int main()
{
    game hexagon(connection::instance(), 0, 0, 600, 480, false);

    int simulate_main_loop = 1;
    emscripten_set_main_loop_arg(game_loop, &hexagon, -1, simulate_main_loop);

    return 0;
}
