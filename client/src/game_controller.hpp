//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GAME_CONTROLLER_H_
#define HEXAGON_CLIENT_GAME_CONTROLLER_H_

#include <variant>

#include <hexagon/model/world.hpp>
#include <hexagon/protocol/message.hpp>

#include "battle_controller.hpp"
#include "connecting_controller.hpp"
#include "world_controller.hpp"

namespace hexagon::client
{
    class mouse;
    class graphics;
}  // namespace hexagon::client

namespace hexagon::client
{
    using game_state = std::variant<  //
        connecting_controller,        //
        world_controller,             //
        battle_controller             //
        >;

    class game_controller
    {
       private:
        game_state state_;
        bool updated_ = true;

       public:
        explicit game_controller(connecting_facet);

       public:  // game controller visiting messages
        void update(protocol::server_message msg);
        void update(const mouse&);
        void resize(int w, int h);

       public:
        void draw(graphics&);
        bool updated() const noexcept;

       public:
        void to_world(world_controller);
        void to_battle(battle_controller);
    };
}  // namespace hexagon::client

#endif
