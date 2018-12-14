//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_CONTROLLER_H_
#define HEXAGON_CLIENT_BATTLE_CONTROLLER_H_

#include <variant>
#include <vector>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/team.hpp>

#include "moving_controller.hpp"
#include "moved_controller.hpp"

namespace hexagon::client
{
    class battle_controller;

    using battle_state = std::variant<  //
        moving_controller,              //
        moved_controller>;

    class battle_controller
    {
        battle_state state_;

       public:
        battle_controller(battle_facet, model::battle, std::size_t) noexcept;

       public:
        void update(const mouse&) noexcept;

       public:
        void draw(canvas&) const;

       public:
        template <typename T>
        void state(T&& s)
        {
            state_ = std::forward<T>(s);
        }
    };
}  // namespace hexagon::client

#endif
