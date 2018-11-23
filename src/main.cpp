#include <emscripten.h>

#include "game.hpp"

void game_loop(void *arg)
{
    hexagon::game *game = static_cast<hexagon::game *>(arg);
    assert(game);
    if (game->running()) {
        if (game->handleEvents()) {
            game->update();
            game->render();
        }
    } else {
        emscripten_cancel_main_loop();
    }
}

int main()
{
    hexagon::game hexagon(0, 0, 600, 480, false);

    int simulate_main_loop = 1;
    emscripten_set_main_loop_arg(game_loop, &hexagon, -1, simulate_main_loop);

    return 0;
}
