//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game.hpp"

#include <iostream>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>

#include <hexagon/model/battle.hpp>

#include <hexagon/protocol/message.hpp>
#include <hexagon/protocol/version_message.hpp>

#include "battle_facet.hpp"
#include "connection.hpp"
#include "game_adapter.hpp"
#include "version.hpp"

using namespace hexagon::client;
using namespace hexagon::sdl;

using namespace hexagon::model;
using namespace hexagon::protocol;

game::game(graphics& g, connection& c)
    : graphics_(g), server_(c), state_{}, facet_(0, 0, g.width(), g.height())
{
    facet_.resize(800, 600);
}

bool game::handleEvents()
{
    SDL_Event event;
    if (1 == SDL_PollEvent(&event)) {
        switch (event.type) {
            default:
                std::cout << "Unhandled event" << event.type << '\n';
                break;
            case SDL_MOUSEMOTION:
                mouse_.event(event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                mouse_.event(event.button);
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        std::cout << "--- Resized\n";
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                return false;
                break;
        }
    }

    return true;
}

void game::update()
{
    // process UI input
    mouse_.handle_all([& s = state_, &facet = facet_](const auto& m) {  //
        hexagon::client::update(s, facet, m);
    });

    // process network input
    server_.handle_all([& s = state_, &facet = facet_](auto msg) {  //
        hexagon::client::update(s, facet, std::move(msg));
    });

    const auto w = graphics_.width();
    const auto h = graphics_.height();
    graphics_.handle_all([& facet = facet_, w, h](const auto&) {  //
        std::cout << "Resized to " << w << 'x' << h << '\n';
        facet.resize(w, h);
    });
}

void game::render()
{
    if (state_.updated()) {
        graphics_->set_draw_color(10, 10, 10, 255);
        graphics_->clear();
        hexagon::client::draw(graphics_, facet_, state_);
        graphics_->present();
        state_.toggle_updated();
    }
}

void game::clean() {}
