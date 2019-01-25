//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/state/battling_state.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <variant>

using namespace hexagon::state;
using namespace hexagon::model;

namespace
{
    team& find_team(battle& b, std::size_t tid)
    {
        auto it = std::find_if(b.teams().begin(), b.teams().end(),
                               [tid](const auto& t) { return t.id == tid; });
        assert(it != b.teams().end());
        return *it;
    }
}  // namespace

battling_state::battling_state(battle b, std::size_t tid)
    : battle_{std::move(b)},
      model_{std::in_place_type<units_joining>, find_team(battle_, tid)}
{
    if (battle_.ready()) start();
}

battling_state::type& battling_state::raw() noexcept { return model_; }
const battling_state::type& battling_state::raw() const noexcept
{
    return model_;
}

battle& battling_state::get_battle() noexcept { return battle_; }
const battle& battling_state::get_battle() const noexcept { return battle_; }

std::size_t battling_state::team_id() const noexcept
{
    return std::visit([](const auto& s) { return s.my_team().id; }, model_);
}

void battling_state::start()
{
    units_joining* jstate = std::get_if<units_joining>(&model_);
    if (jstate) {
        if (jstate->my_team().units.empty())
            std::cerr << "ERROR: entering move phase without units!\n";
        else {
            unit_moving nstate(battle_, *jstate);
            model_ = std::move(nstate);
        }
    } else {
        std::cerr << "ERROR: battle must start from units_joining state\n";
    }
}

bool battling_state::joining() const noexcept
{
    return std::holds_alternative<units_joining>(model_);
}

bool battling_state::moving() const noexcept
{
    return std::holds_alternative<unit_moving>(model_);
}

bool battling_state::moved() const noexcept
{
    return std::holds_alternative<units_moved>(model_);
}
