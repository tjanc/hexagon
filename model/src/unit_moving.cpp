//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/unit_moving.hpp>

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>
#include <hexagon/model/unit.hpp>
#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/unit_io.hpp>

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
        using namespace hexagon::protocol::io;

        std::cout << "Generate reach map at " << center << '\n';

        unit_moving::reach_map result{
            unit_moving::reach_map::tiles_container(m.size(), 0), m.width()};

        assert(contains(m, center));
        const tile& center_tile = m.at(center);

        assert(center_tile.get_if_unit());
        auto& u = *center_tile.get_if_unit();

        std::deque<basic_map_index> idxs;
        idxs.emplace_back(center);

        result.at(center) = u.range();
        std::cout << "Stats " << u.statistics() << '\n';

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

    battle init_explore(battle b, std::size_t tidx, std::size_t uidx) noexcept
    {
        using namespace hexagon::protocol::io;

        const auto& t = *(b.teams().begin() + tidx);
        [](const auto& t) {
            std::cout << "Your team has " << t.units.size() << " units:\n";

            using namespace hexagon::protocol::io;
            for (const auto& u : t.units) std::cout << " - " << u << '\n';
        }(t);

        std::cout << "The map has " << b.get_map().width() << " columns and "
                  << b.get_map().height() << " rows\n";

        const auto& u = *(t.units.begin() + uidx);
        std::cout << "Preparing for moving unit " << u << '\n';

        auto it = find_unit(b.get_map(), u);
        if(contains(b.get_map(), it))
            std::cout << "Found referenced unit.\n";
        else
            std::cerr << "Referenced unit not found.\n";

        return std::move(b);
    }
}  // namespace

unit_moving::unit_moving(battle b, std::size_t tidx, std::size_t uidx) noexcept
    : model_{init_explore(std::move(b), tidx, uidx)},
      team_{std::next(model_.teams().begin(), tidx)},
      unit_{std::next(team_->units.begin(), uidx)},
      unit_position_{find_unit(model_.get_map(), *unit_)},
      reach_map_{generate_reach_map(model_.get_map(), unit_position_)}
{
    std::cout << "Unit moving loaded.\n";
}

unit_moving::unit_moving(battle b, std::size_t tidx) noexcept
    : unit_moving(std::move(b), tidx, 0)
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
    void move_unit(unit_moving& model, basic_map_index idx) noexcept
    {
        using namespace hexagon::protocol::io;

        std::cout << "Moving unit from " << model.unit_position_ << " to "
                  << idx << '\n';

        auto& m = model.model_.get_map();

        auto& source = m.at(model.unit_position_);
        auto& target = m.at(idx);

        auto* u = source.detach_unit();
        assert(u);

        assert(target.empty());
        target.attach(*u);

        ++model.unit_;
        if (model.team_->units.end() == model.unit_) {
            auto ntidx = model.team_ - model.model_.teams().begin();
            unit_moving n(std::move(model.model_), ntidx);
            model = std::move(n);

        } else {
            auto ntidx = model.team_ - model.model_.teams().begin();
            auto nuidx = model.unit_ - model.team_->units.begin();
            unit_moving n(std::move(model.model_), ntidx, nuidx);
            model = std::move(n);
        }
    }
}  // namespace hexagon::model

const team& unit_moving::my_team() const noexcept { return *team_; }

team& unit_moving::my_team() noexcept { return *team_; }
