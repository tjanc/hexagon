//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/battle_io.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

#include <hexagon/protocol/io/map_io.hpp>
#include <hexagon/protocol/io/team_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

namespace
{
    unit* find_unit(battle::team_container& teams, std::size_t unit_id) noexcept
    {
        for (auto& t : teams) {
            for (auto& u : t.units) {
                if (u.id() == unit_id) return &u;
            }
        }
        return nullptr;
    }

    std::size_t count_units(const map& m) noexcept
    {
        return std::count_if(m.begin(), m.end(),
                             [](const auto& t) { return t.has_unit(); });
    }
}  // namespace

std::istream& io::operator>>(std::istream& in, battle& obj)
{
    std::size_t n;
    in >> n;

    assert(n < 10);
    battle::team_container teams;

    for (std::size_t i = 0; i < n; ++i) {
        team t;
        if (in >> t) teams.emplace_back(std::move(t));
    }

    map m;
    in >> m;

    std::size_t n_pos;
    in >> n_pos;

    assert(n_pos < 30);

    for (std::size_t i = 0; i < n_pos; ++i) {
        std::size_t unit_id;
        in >> unit_id;

        basic_map_index position;
        in >> position;

        if (!contains(m, position)) {
            std::cout << "WARN: position out of bounds `" << position << "`\n";
            continue;
        }

        unit* u = find_unit(teams, unit_id);
        if (!u) {
            std::cout << "WARN: referenced unit not found `" << unit_id
                      << "`\n";
            continue;
        }

        m.at(position).attach(*u);
    }

    obj = std::move(battle{std::move(m), std::move(teams)});

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const model::battle& obj)
{
    out << obj.teams().size() << ' ';
    for (const auto& t : obj.teams()) out << t;

    const auto& m = obj.get_map();
    out << m << ' ';

    std::size_t n_pos = count_units(m);
    out << n_pos << ' ';

    iterate(m, [&out](const auto& t, const auto& idx) {
        const unit* u = t.get_if_unit();
        if (u) {
            out << u->id() << ' '  //
                << idx << ' ';
        }
    });

    return out;
}