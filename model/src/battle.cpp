//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/battle.hpp>

#include <algorithm>
#include <cassert>

using namespace hexagon::model;

battle::battle(map m, std::size_t full)
    : battle(std::move(m), full, battle::team_container{})
{
}

battle::battle(map m, std::size_t full, battle::team_container teams)
    : map_{std::move(m)},
      full_teams_{full},
      teams_{std::move(teams)},
      max_units_{2}
{
}

team& battle::join(const team& t)
{
    if (t.units.size() > max_units_) {
        team::unit_container used_units;
        std::copy(t.units.begin(), std::next(t.units.begin(), max_units_),
                  std::back_inserter(used_units));
        teams_.emplace_back(t.id, std::move(used_units));
    } else {
        teams_.emplace_back(t);
    }

    for (auto& u : teams_.back().units)
        if (map_.end() == spawn(map_, u)) break;

    return teams_.back();
}

team battle::leave(int tid)
{
    const auto is_team = [tid](const team& t) { return t.id == tid; };

    const auto team_it = std::find_if(teams_.begin(), teams_.end(), is_team);
    assert(team_it != teams_.end());
    iterate(map_, [t = *team_it](auto& tle, auto idx) {
        const unit* u = tle.get_if_unit();
        if (u) {
            if (t.end() != std::find_if(t.begin(), t.end(),
                                        [uid = u->id()](const unit& u) {
                                            return u.id() == uid;
                                        })) {
                tle.detach_unit();
            }
        }
    });
    team result = *team_it;
    teams_.erase(team_it);

    return std::move(result);
}

battle::team_container& battle::teams() noexcept { return teams_; }

const battle::team_container& battle::teams() const noexcept { return teams_; }

map& battle::get_map() noexcept { return map_; }

const map& battle::get_map() const noexcept { return map_; }

bool battle::ready() const noexcept { return teams().size() == full(); }

std::size_t battle::full() const noexcept { return full_teams_; }
