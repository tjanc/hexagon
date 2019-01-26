//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/state/local_state.hpp>

using namespace hexagon::model;
using namespace hexagon::state;

local_state::local_state() noexcept
    : state_{connecting_state{}}, updated_{false}
{
}

world_state& local_state::to_world(world_state w)
{
    state_ = std::move(w);
    return std::get<world_state>(state_);
}

battling_state& local_state::to_battle(battling_state s)
{
    state_ = std::move(s);
    return std::get<battling_state>(state_);
}

const local_state::type& local_state::raw() const noexcept { return state_; }
local_state::type& local_state::raw() noexcept { return state_; }

bool local_state::updated() const noexcept { return updated_; }

void local_state::toggle_updated() noexcept { updated_ = !updated_; }
