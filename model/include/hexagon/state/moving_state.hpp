//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_MOVING_STATE_H_
#define HEXAGON_STATE_MOVING_STATE_H_

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map.hpp>
#include <hexagon/model/unit_moving.hpp>

namespace hexagon::state
{
    class moving_state
    {
       public:
        using type = model::unit_moving;

       private:
        type model_;

       public:
        void move(model::battle& b, model::basic_map_index idx);
    };

}  // namespace hexagon::state

#endif
