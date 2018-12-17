//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_CONTROLLER_H_
#define HEXAGON_CLIENT_BATTLE_CONTROLLER_H_

#include <variant>
#include <vector>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/team.hpp>

#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>

#include "battle_facet.hpp"

namespace hexagon::client
{
    class mouse;
}

namespace hexagon::client
{
    class battle_controller
    {
       public:
        using model = std::variant<  //
            model::unit_moving,      //
            model::units_moved>;

       private:
        model state_;
        battle_facet facet_;

       public:
        battle_controller(battle_facet, hexagon::model::battle,
                          std::size_t) noexcept;

       public:
        battle_facet& facet() noexcept;

       public:
        void update(const mouse&) noexcept;

       public:
        void draw(graphics&) const;
    };
}  // namespace hexagon::client

#endif
