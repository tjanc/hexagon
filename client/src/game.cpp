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

namespace
{
    struct handle_events_visitor {
        battle_facet& battle_facet_;
        bool operator()(battle_controller& projection)
        {
            SDL_Event event;
            if (1 == SDL_PollEvent(&event)) {
                switch (event.type) {
                    default:
                        break;
                    case SDL_MOUSEMOTION:
                        battle_facet_.mouse_move(projection);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        battle_facet_.mouse_down(projection);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        battle_facet_.mouse_up(projection);
                        break;
                }
            }
            return true;
        }

        bool operator()(world_controller& projection)
        {
            assert(!"world view not implemented yet");
            return false;
        }

        bool operator()(connecting_controller& projection)
        {
            // do nothing until connected
            return true;
        }
    };

    struct render_visitor {
        battle_facet& battle_facet_;
        renderer& renderer_;
        void operator()(connecting_controller& projection)
        {
            // do nothing until connected
        }

        void operator()(battle_controller& projection)
        {
            battle_facet_.draw(renderer_, projection);
        }

        void operator()(world_controller&)
        {
            assert(!"world view not implemented yet");
        }
    };

}  // namespace

game::game(int x, int y, int width, int height, bool fullscreen)
    : graphics_(),
      window_(graphics_, "Hexagon " HEXAGON_CLIENT_VERSION, x, y, width, height,
              fullscreen),
      renderer_(window_),
      game_controller_(),
      battle_(renderer_)
{
    if (!renderer_ || !window_) {
        std::cerr << "unable to create window/renderer";
        running_ = false;
        return;
    }
}

bool game::handleEvents()
{
    return visit(handle_events_visitor{battle_}, game_controller_);
}

void game::update()
{
    connection::instance().handle_all(game_controller_);
}

void game::render()
{
    renderer_.set_draw_color(10, 10, 10, 255);
    renderer_.clear();

    visit(render_visitor{battle_, renderer_}, game_controller_);

    renderer_.present();
}

void game::clean() {}
