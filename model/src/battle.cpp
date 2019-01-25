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

battle::battle(const battle& rhs)
    : battle(rhs.map_, rhs.full_teams_, rhs.teams_)
{
    *this = rhs;
}

battle& battle::operator=(const battle& rhs)
{
    map_ = rhs.map_;
    full_teams_ = rhs.full_teams_;
    teams_ = rhs.teams_;
    max_units_ = rhs.max_units_;

    for (team& t : teams_)
        for (auto& spot : map_)
            if (spot.has_unit()) {
                unit* o = spot.detach_unit();
                auto n = std::find_if(t.units.begin(), t.units.end(),
                                      [uid = o->id()](const unit& u) {  //
                                          return u.id() == uid;
                                      });
                if (n == t.units.end())
                    // if not in this team, it's in another
                    spot.attach(*o);
                else
                    spot.attach(*n);
            }

    return *this;
}

std::pair<team*, battle::placement_container> battle::join(const team& t)
{
    if (t.units.size() > max_units_) {
        team::unit_container used_units;
        std::copy(t.units.begin(), std::next(t.units.begin(), max_units_),
                  std::back_inserter(used_units));
        teams_.emplace_back(t.id, std::move(used_units));
    } else {
        teams_.emplace_back(t);
    }

    battle::placement_container placements;
    for (auto& u : teams_.back().units) {
        auto it = spawn(map_, u);
        if (map_.end() == it) break;
        placements.emplace_back(to_index(map_, it));
    }

    return {&teams_.back(), std::move(placements)};
}

team& battle::join(const team& ot, const battle::placement_container& placements)
{
    auto& t = teams().emplace_back(ot);

    auto& field = get_map();
    auto udx = t.units.begin();
    for (const auto& placement : placements) {
        if (udx == t.units.end())
            return t;

        unit& u = *udx;

        if (!contains(field, placement))
            return t;

        tile& target = field.at(placement);
        target.attach(u);
        ++udx;
    }

    return t;
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
