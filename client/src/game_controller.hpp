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
    struct connecting_controller {
    };

    class game_controller
    {
        using state = std::variant<  //
            connecting_controller,   //
            world_controller,        //
            battle_controller        //
            >;

       private:
        state state_;

       public:
        game_controller();

        template <typename Visitor>
        friend decltype(auto) visit(Visitor&& v, game_controller& self)
        {
            return std::visit(std::forward<Visitor>(v), self.state_);
        }

        void enter_battle(hexagon::model::battle b, hexagon::model::team t);

       public:
        void operator()(const hexagon::protocol::version_response& msg);
        void operator()(const hexagon::protocol::map_response& msg);
        void operator()(const hexagon::protocol::unknown_message& msg);
    };
}  // namespace hexagon::client

#endif
