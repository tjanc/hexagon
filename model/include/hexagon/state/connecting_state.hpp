//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_CONNECTING_STATE_H_
#define HEXAGON_STATE_CONNECTING_STATE_H_

#include <hexagon/model/connecting.hpp>

namespace hexagon::state
{
    class connecting_responder
    {
    };

    class connecting_state
    {
       public:
        using type = model::connecting;

       private:
        type model_;
        connecting_responder responder_;
    };

}  // namespace hexagon::state

#endif
