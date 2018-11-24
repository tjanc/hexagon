#include "game.hpp"

#include <iostream>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map_loader.hpp>

#include "battle_facet.hpp"

using namespace hexagon;

namespace
{
    struct handle_events_visitor {
        battle_facet& battle_facet_;
        bool operator()(model::battle_projection& projection)
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

        bool operator()(model::world_projection& projection)
        {
            assert(!"world view not implemented yet");
            return false;
        }
    };

    struct render_visitor {
        battle_facet& battle_facet_;
        sdl::renderer& renderer_;
        void operator()(model::battle_projection& projection)
        {
            battle_facet_.draw(renderer_, projection);
        }

        void operator()(model::world_projection&)
        {
            assert(!"world view not implemented yet");
        }
    };

}  // namespace

game::game(int x, int y, int width, int height, bool fullscreen)
    : graphics_(),
      window_(graphics_, "Hexagon 0.1", x, y, width, height, fullscreen),
      renderer_(window_),
      battle_model_(load_map("assets/map1.map")),
      game_projection_(battle_model_, model::team{}),
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
    using namespace hexagon::model;
    return visit(handle_events_visitor{battle_}, game_projection_);
}

void game::update() {}

void game::render()
{
    renderer_.set_draw_color(10, 10, 10, 255);
    renderer_.clear();

    using namespace hexagon::model;
    visit(render_visitor{battle_, renderer_}, game_projection_);

    renderer_.present();
}

void game::clean() {}
