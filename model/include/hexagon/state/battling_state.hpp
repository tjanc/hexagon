//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_BATTLING_STATE_H_
#define HEXAGON_STATE_BATTLING_STATE_H_

#include <iostream> // XXX debug
#include <cstdint>
#include <variant>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>

//#include "moved_state.hpp"
//#include "moving_state.hpp"

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

       public:
        battling_state(model::battle b, const model::team& t)
            : battle_{std::move(b)},
              model_{model::unit_moving{battle_, battle_.join(t)}}
        {
            std::cout << "=== ENTERED BATTLING STATE ===\n";
            std::cout << "=== units on battlefield: "
                      << std::count_if(
                             battle_.get_map().begin(), battle_.get_map().end(),
                             [](const auto& el) { return el.has_unit(); })
                      << '\n';
        }

       public:
        type& raw() noexcept { return model_; }
        const type& raw() const noexcept { return model_; }

       public:
        model::battle& get_battle() noexcept { return battle_; }
        const model::battle& get_battle() const noexcept { return battle_; }
    };
}  // namespace hexagon::state

#endif
