//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "local_state.hpp"

using namespace hexagon::server;
using namespace hexagon::model;

local_state::local_state() : state_{connecting{}} {}

void local_state::to_world(world w) { state_ = std::move(w); }

void local_state::to_battle(model::battle& b, std::size_t tidx)
{
    state_ = unit_moving{b, tidx};
}

local_state::type& local_state::raw() noexcept { return state_; }
const local_state::type& local_state::raw() const noexcept { return state_; }
