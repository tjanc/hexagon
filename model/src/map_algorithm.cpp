//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/map_algorithm.hpp>

#include <algorithm>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>
#include <hexagon/model/tile.hpp>
#include <hexagon/model/unit.hpp>

using namespace hexagon::model;

namespace
{
    void generate_reach_map_step(reach_map& result,
                                 std::deque<basic_map_index>& queue,
                                 const map& m, const unit& u,
                                 basic_map_index idx, basic_map_index nidx)
    {
        if (contains(result, nidx) && result.at(nidx) == 0) {
            const auto& source = m.at(idx);
            const auto& target = m.at(nidx);

            const auto vmove_max = move_vertical_max(u.statistics());

            const int penalty = move_penalty(u.statistics(), source.elevation(),
                                             target.elevation());
            int nrange = static_cast<int>(result.at(idx)) - penalty;
            if (nrange > 0) {
                result.at(nidx) = nrange;
                queue.emplace_back(nidx);
            }
        }
    }
}  // namespace

reach_map hexagon::model::generate_reach_map(const map& m,
                                             basic_map_index center)
{
    reach_map result{reach_map::tiles_container(m.size(), 0), m.width()};

    assert(contains(m, center));
    const tile& center_tile = m.at(center);

    assert(center_tile.get_if_unit());
    auto& u = *center_tile.get_if_unit();

    std::deque<basic_map_index> idxs;
    idxs.emplace_back(center);

    result.at(center) = u.status().stamina;

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

std::pair<vertex_path::const_iterator, std::uint16_t>
hexagon::model::traversal_cost(const map& m, const unit& u,
                               vertex_path::const_iterator source,
                               vertex_path::const_iterator e)
{
    if (source == e) return std::make_pair(e, 0);

    auto stamina = u.status().stamina;
    std::cout << "INFO: traversal_cost:stamina=" << stamina << '\n';
    auto last = source;
    if (last != e)
        for (auto it = last + 1; it != e; ++it, ++last) {
            std::cout << "INFO: === traverse step " << last->x << 'x' << last->y
                      << '\n';
            const auto& it_tile = m.at(*it);
            const auto& last_tile = m.at(*last);
            if (!it_tile.empty()) {
                std::cout << "INFO: === ending traversal; collision\n";
                break;
            }
            const int penalty = move_penalty(
                u.statistics(), last_tile.elevation(), it_tile.elevation());
            if (penalty > stamina) {
                std::cout << "INFO: === ending traversal; out of stamina\n";
                break;
            }
            stamina -= penalty;
        }

    return std::make_pair(last, u.status().stamina - stamina);
}

std::pair<vertex_path::const_iterator, std::uint16_t>
hexagon::model::traverse_unit(map& m, vertex_path::const_iterator source,
                              vertex_path::const_iterator e)
{
    if (source == e) {
        std::cerr << "ERROR: empty path\n";
        return std::make_pair(e, 0);
    }

    auto* u = m.at(*source).get_if_unit();
    if (!u) {
        std::cerr << "ERROR" << __LINE__ << ": no unit at source\n";
        return std::make_pair(e, 0);
    }

    auto [tgt, stamina] = traversal_cost(m, *u, source, e);

    if (!move_unit(m, *source, *tgt, stamina)) {
        std::cerr << "ERROR: unable to move unit\n";
        return std::make_pair(e, 0);
    }

    return std::make_pair(tgt, stamina);
}

unit* hexagon::model::move_unit(map& m, basic_map_index src,
                                basic_map_index tgt,
                                std::uint16_t stamina_cost) noexcept
{
    auto* u = m.at(src).detach_unit();
    if (!u) {
        std::cerr << "ERROR" << __LINE__ << ": no unit at source (" << src.x
                  << 'x' << src.y << ")\n";
        return nullptr;
    }

    auto& tgt_t = m.at(tgt);
    if (!tgt_t.empty()) {
        std::cerr << "ERROR: target not empty\n";
        return nullptr;
    }

    u->status().stamina -= stamina_cost;
    tgt_t.attach(*u);
    return u;
}
