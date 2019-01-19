//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_STATE_WORLD_STATE_H_
#define HEXAGON_STATE_WORLD_STATE_H_

#include <hexagon/model/world.hpp>

namespace hexagon::state
{
    class world_state
    {
       public:
        using type = model::world;

       private:
        type model_;

       public:
        explicit world_state(type m) : model_(std::move(m)) {}

        const type& raw() const noexcept { return model_; }
        type& raw() noexcept { return model_; }
    };

}  // namespace hexagon::state

#endif
