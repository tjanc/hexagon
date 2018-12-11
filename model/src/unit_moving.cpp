//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/unit_moving.hpp>

#include <algorithm>
#include <cassert>
#include <deque>

#include <hexagon/model/map.hpp>
#include <hexagon/model/unit.hpp>

using namespace hexagon::model;

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

unit_moving::unit_moving(battle b, battle::team_container::iterator t,
                         team::unit_container::iterator u) noexcept
    : model_{std::move(b)},
      team_{t},
      unit_{u},
      unit_position_{find_unit(model_.get_map(), *u)},
      reach_map_{generate_reach_map(model_.get_map(), unit_position_)}
{
}

unit_moving::unit_moving(battle b, battle::team_container::iterator t) noexcept
    : unit_moving(std::move(b), t, t->units.begin())
{
}

const battle& unit_moving::battlefield() const noexcept { return model_; }

battle& unit_moving::battlefield() noexcept { return model_; }

basic_map_index unit_moving::position() const noexcept
{
    return unit_position_;
}

bool unit_moving::reachable(basic_map_index idx) const noexcept
{
    if (!contains(reach_map_, idx)) return false;

    if (idx == unit_position_) return true;

    return model_.get_map().at(idx).empty() && reach_map_.at(idx) > 0;
}

bool unit_moving::has_next() const noexcept
{
    return std::next(unit_) != team_->units.end();
}

namespace hexagon::model
{
    unit_moving move_unit(unit_moving model, basic_map_index idx) noexcept
    {
        auto& m = model.model_.get_map();

        auto& source = m.at(model.unit_position_);
        auto& target = m.at(idx);

        auto* u = source.detach_unit();
        assert(u);

        assert(target.empty());
        target.attach(*u);

        ++model.unit_;
        if (model.team_->units.end() == model.unit_) {
            return unit_moving(std::move(model.model_), model.team_);
        }

        return unit_moving(std::move(model.model_), model.team_, model.unit_);
    }
}  // namespace hexagon::model

const team& unit_moving::my_team() const noexcept { return *team_; }

team& unit_moving::my_team() noexcept { return *team_; }
