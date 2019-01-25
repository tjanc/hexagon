//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/unit_moving.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>
#include <hexagon/model/unit.hpp>
#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/unit_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

namespace
{
    void generate_reach_map_step(unit_moving::reach_map& result,
                                 std::deque<basic_map_index>& queue,
                                 const map& m, const unit& u,
                                 basic_map_index idx, basic_map_index nidx)
    {
        if (contains(result, nidx) && result.at(nidx) == 0) {
            const auto& source = m.at(idx);
            const auto& target = m.at(nidx);

            const auto d_elev = target.elevation() - source.elevation();

            if (d_elev < u.vmove_max() && d_elev > -u.vmove_max()) {
                auto penalty = u.hmove_penalty() +
                               (d_elev > 0 ? d_elev * u.vmove_penalty() : 0);
                int nrange = static_cast<int>(result.at(idx)) - penalty;
                if (nrange > 0) {
                    result.at(nidx) = nrange;
                    queue.emplace_back(nidx);
                }
            }
        }
    }

    unit_moving::reach_map generate_reach_map(const map& m,
                                              basic_map_index center)
    {
        using namespace hexagon::protocol::io;

        unit_moving::reach_map result{
            unit_moving::reach_map::tiles_container(m.size(), 0), m.width()};

        assert(contains(m, center));
        const tile& center_tile = m.at(center);

        assert(center_tile.get_if_unit());
        auto& u = *center_tile.get_if_unit();

        std::deque<basic_map_index> idxs;
        idxs.emplace_back(center);

        result.at(center) = u.range();

        while (!idxs.empty()) {
            const auto& idx = idxs.front();
            idxs.pop_front();

            generate_reach_map_step(result, idxs, m, u, idx, west(idx));
            generate_reach_map_step(result, idxs, m, u, idx, north_west(idx));
            generate_reach_map_step(result, idxs, m, u, idx, north_east(idx));
            generate_reach_map_step(result, idxs, m, u, idx, east(idx));
            generate_reach_map_step(result, idxs, m, u, idx, south_west(idx));
            generate_reach_map_step(result, idxs, m, u, idx, south_east(idx));
        }

        return result;
    }
}  // namespace

unit_moving::unit_moving(battle& b, team& t,
                         team::unit_container::iterator uidx) noexcept
    : start_{std::chrono::steady_clock::now()},
      movements_{},
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
    : unit_moving(b, t, t.units.begin())
{
}

unit_moving::unit_moving(battle& b, units_joining& prev) noexcept
    : unit_moving(b, prev.my_team(), prev.my_team().units.begin())
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

bool unit_moving::has_next() const noexcept
{
    return std::next(unit_) != team_->units.end();
}

void unit_moving::next(battle& b)
{
    assert(has_next());
    ++unit_;

    *this = unit_moving{b, *team_, unit_};
}

void unit_moving::move(map& m, basic_map_index idx)
{
    using namespace hexagon::protocol::io;

    std::cout << "Moving unit from " << unit_position_ << " to " << idx
              << " after "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start_)
                     .count()
              << "ms\n";

    movements_.emplace_back(std::chrono::steady_clock::now() - start_,
                            unit_position_, idx);
    move_unit(m, unit_position_, idx);
}

const team& unit_moving::my_team() const noexcept { return *team_; }

team& unit_moving::my_team() noexcept { return *team_; }
