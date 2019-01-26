//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_BATTLING_STATE_H_
#define HEXAGON_STATE_BATTLING_STATE_H_

#include <hexagon/model/battle.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_joining.hpp>
#include <hexagon/model/units_moved.hpp>
#include <variant>

namespace hexagon::state
{
    class battling_state
    {
       public:
        using type = std::variant<  //
            model::units_joining,   //
            model::unit_moving,     //
            model::units_moved      //
            >;

       private:
        /// common state
        model::battle battle_;

        /// more specific state
        type model_;

       public:
        battling_state(model::battle b, std::size_t tid);

       public:
        type& raw() noexcept;
        const type& raw() const noexcept;

       public:
        model::battle& get_battle() noexcept;
        const model::battle& get_battle() const noexcept;

       public:
        std::size_t team_id() const noexcept;

       public:
        void start();

       public:
        bool joining() const noexcept;
        bool moving() const noexcept;
        bool moved() const noexcept;
    };
}  // namespace hexagon::state

#endif
