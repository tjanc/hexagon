//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_BATTLING_STATE_H_
#define HEXAGON_STATE_BATTLING_STATE_H_

#include <cstdint>
#include <variant>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>

namespace hexagon::state
{
    class battling_state
    {
       public:
        using type = std::variant<  //
            model::unit_moving,     //
            model::units_moved      //
            >;

       private:
        model::battle battle_;
        type model_;
        std::size_t team_id_;

       public:
        battling_state(model::battle b, std::size_t tidx)
            : battle_{std::move(b)},
              model_{model::unit_moving{battle_, tidx}},
              team_id_{tidx}
        {
        }

       public:
        type& raw() noexcept { return model_; }
        const type& raw() const noexcept { return model_; }

       public:
        model::battle& get_battle() noexcept { return battle_; }
        const model::battle& get_battle() const noexcept { return battle_; }

       public:
        std::size_t team_id() const noexcept { return team_id_; }
    };
}  // namespace hexagon::state

#endif
