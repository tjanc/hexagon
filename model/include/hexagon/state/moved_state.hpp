//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_MOVED_STATE_H_
#define HEXAGON_STATE_MOVED_STATE_H_

#include <hexagon/model/units_moved.hpp>

namespace hexagon::state
{
    class moved_responder
    {
    };

    class moved_state
    {
       public:
        using type = model::units_moved;

       private:
        type model_;
        moved_responder responder_;
    };

}  // namespace hexagon::state

#endif
