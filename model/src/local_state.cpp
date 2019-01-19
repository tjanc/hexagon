//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/state/local_state.hpp>

using namespace hexagon::model;
using namespace hexagon::state;

local_state::local_state() : state_{connecting_state{}} {}

world_state& local_state::to_world(world w)
{
    state_ = world_state{std::move(w)};
    return std::get<world_state>(state_);
}

battling_state& local_state::to_battle(battle b, const team& t)
{
    state_ = battling_state{std::move(b), t};
    return std::get<battling_state>(state_);
}

local_state::type& local_state::raw() noexcept { return state_; }
const local_state::type& local_state::raw() const noexcept { return state_; }
