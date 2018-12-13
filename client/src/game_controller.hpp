//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GAME_CONTROLLER_H_
#define HEXAGON_CLIENT_GAME_CONTROLLER_H_

#include <variant>

#include <hexagon/protocol/message.hpp>

#include "battle_controller.hpp"
#include "world_controller.hpp"

namespace hexagon::client
{
    class mouse;
    class canvas;
}  // namespace hexagon::client

namespace hexagon::client
{
    struct connecting_controller {
    };

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
        game_controller();

       public:  // game controller visiting messages
        void update(hexagon::protocol::version_response);
        void update(hexagon::protocol::login_response);
        void update(hexagon::protocol::map_response);
        void update(hexagon::protocol::battle_message);
        void update(hexagon::protocol::unknown_message);
        void update(const mouse&);

       public:
        void draw(canvas&);
        bool updated() const noexcept;

       public:
        template <typename T>
        void state(T&& s)
        {
            state_ = std::forward<T>(s);
            updated_ = true;
        }
    };
}  // namespace hexagon::client

#endif
