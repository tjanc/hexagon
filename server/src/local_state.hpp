//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_LOCAL_STATE_H_
#define HEXAGON_SERVER_LOCAL_STATE_H_

#include <cstdint>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/connecting.hpp>
#include <hexagon/model/unit_moving.hpp>
#include <hexagon/model/units_moved.hpp>
#include <hexagon/model/world.hpp>

namespace hexagon::server
{
    class local_state
    {
       public:
        using battling = std::variant<  //
            model::unit_moving,         //
            model::units_moved          //
            >;

        using type = std::variant<  //
            model::connecting,      //
            model::world,           //
            battling                //
            >;

       private:
        type state_;

       public:
        local_state();

       public:
        void to_world(model::world);
        void to_battle(model::battle&, std::size_t tidx);

       public:
        type& raw() noexcept;
        const type& raw() const noexcept;
    };
}  // namespace hexagon::server

#endif
