//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/unit_moving.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>
#include <hexagon/model/path.hpp>
#include <hexagon/model/unit.hpp>
#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/unit_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

unit_moving::unit_moving(battle& b, team& t,
                         team::unit_container::iterator uidx,
                         unit_moving::movement_container movements) noexcept
    : start_{std::chrono::steady_clock::now()},
      movements_{std::move(movements)},
      team_{&t},
      unit_{uidx},
      unit_position_{find_unit(b.get_map(), unit_->id())},
      reach_map_{generate_reach_map(b.get_map(), unit_position_)}
{
    std::cout << "INFO: moving unit " << team_->id << ':' << unit_->id()
              << " in [";
    for (const auto& u : team_->units) std::cout << u.id() << ',';
    std::cout << "]\n";
}

unit_moving::unit_moving(battle& b, team& t) noexcept
    : unit_moving(b, t, t.units.begin(), unit_moving::movement_container{})
{
}

unit_moving::unit_moving(battle& b, units_joining& prev) noexcept
    : unit_moving(b, prev.my_team(), prev.my_team().units.begin(),
                  unit_moving::movement_container{})
{
}

basic_map_index unit_moving::position() const noexcept
{
    return unit_position_;
}

bool unit_moving::reachable(const map& m, basic_map_index idx) const noexcept
{
    if (!contains(reach_map_, idx)) return false;

    if (idx == unit_position_) return true;

    return m.at(idx).empty() && reach_map_.at(idx) > 0;
}

bool unit_moving::end() const noexcept { return unit_ == team_->units.end(); }

void unit_moving::next(map& m) noexcept
{
    assert(!end());
    ++unit_;
    if (!end()) {
        unit_position_ = find_unit(m, unit_->id());
        reach_map_ = generate_reach_map(m, unit_position_);
    }
}

bool unit_moving::follow(map& m, vertex_path path)
{
    if (reachable(m, *path.target())) {
        movements_.emplace_back(std::chrono::steady_clock::now() - start_,
                                std::move(path));
        next(m);
        return true;
    }

    return false;
}

const reach_map& unit_moving::reaches() const noexcept { return reach_map_; }

const team& unit_moving::my_team() const noexcept { return *team_; }

team& unit_moving::my_team() noexcept { return *team_; }
