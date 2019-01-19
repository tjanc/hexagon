//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_LOCAL_STATE_H_
#define HEXAGON_STATE_LOCAL_STATE_H_

#include <algorithm>
#include <memory>
#include <variant>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/player.hpp>
#include <hexagon/model/team.hpp>
#include <hexagon/model/world.hpp>

#include <hexagon/state/battling_state.hpp>
#include <hexagon/state/connecting_state.hpp>
#include <hexagon/state/world_state.hpp>

namespace hexagon::state
{
    class local_state
    {
       public:
        using type = std::variant<  //
            connecting_state,       //
            world_state,            //
            battling_state          //
            >;

       private:
        type state_;

       public:
        local_state();

       public:
        world_state& to_world(model::world w);
        battling_state& to_battle(model::battle b, const model::team& t);

       public:
        type& raw() noexcept;
        const type& raw() const noexcept;
    };
}  // namespace hexagon::state

#endif
