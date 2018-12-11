//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/battle.hpp>

#include <cassert>

using namespace hexagon::model;

battle::battle(map m) : map_{m} {}

battle::team_container::iterator battle::join(team t)
{
    teams_.emplace_back(std::move(t));

    for (auto& u : teams_.back().units)
        if (map_.end() == spawn(map_, u)) break;

    return --teams_.end();
}

team battle::leave(battle::team_container::iterator it)
{
    assert(false);
    // map_.withdraw(it->u);

    team result = std::move(*it);
    teams_.erase(it);
    return result;
}
