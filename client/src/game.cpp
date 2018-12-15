//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game.hpp"

#include <iostream>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map_loader.hpp>

#include <hexagon/protocol/message.hpp>
#include <hexagon/protocol/version_message.hpp>

#include "battle_facet.hpp"
#include "connection.hpp"
#include "version.hpp"

using namespace hexagon::client;
using namespace hexagon::sdl;

using namespace hexagon::model;
using namespace hexagon::protocol;

game::game(connection& c, int x, int y, int width, int height, bool fullscreen)
    : server_(c),
      graphics_(),
      window_(graphics_, "Hexagon " HEXAGON_CLIENT_VERSION, x, y, width, height,
              fullscreen),
      canvas_(window_),
      game_controller_(connecting_facet{x, y, width, height})
{
}

bool game::handleEvents()
{
    SDL_Event event;
    if (1 == SDL_PollEvent(&event)) {
        switch (event.type) {
            default:
                break;
            case SDL_MOUSEMOTION:
                mouse_.event(event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                mouse_.event(event.button);
            case SDL_QUIT:
                return false;
        }
    }

    return true;
}

void game::update()
{
    // process UI input
    mouse_.handle_all([& c = game_controller_](const auto& m) {  //
        c.update(m);
    });

    // process network input
    server_.handle_all([& c = game_controller_](auto msg) {  //
        c.update(std::move(msg));
    });
}

void game::render()
{
    if (game_controller_.updated()) {
        canvas_->set_draw_color(10, 10, 10, 255);
        canvas_->clear();
        game_controller_.draw(canvas_);
        canvas_->present();
    }
}

void game::clean() {}
