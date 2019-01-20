//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/battle_io.hpp>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/team.hpp>
#include <hexagon/model/map.hpp>
#include <hexagon/model/tile.hpp>
#include <hexagon/model/unit.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

#include <hexagon/protocol/io/message_io.hpp>

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

    std::cout << "DEBUG: n_pos `" << n_pos << "`\n";

    assert(n_pos < 30);

    for (std::size_t i = 0; i < n_pos; ++i) {
        std::size_t unit_id;
        in >> unit_id;

        basic_map_index position;
        in >> position;

        if (!contains(m, position)) {
            std::cerr << "WARN: position out of bounds `" << position << "`\n";
            continue;
        }

        unit* u = find_unit(teams, unit_id);
        if (!u) {
            std::cerr << "WARN: referenced unit not found `" << unit_id
                      << "`\n";
            continue;
        }

        m.at(position).attach(*u);
    }

    obj = std::move(battle{std::move(m), std::move(teams)});

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const battle& obj)
{
    using namespace protocol::io;

    out << obj.teams();

    const auto& m = obj.get_map();
    out << ' ' << m;

    std::size_t n_pos = count_units(m);
    out << ' ' << n_pos;

    iterate(m, [&out](const auto& t, const auto& idx) {
        const unit* u = t.get_if_unit();
        if (u) {
            out << ' ' << u->id()  //
                << ' ' << idx;
        }
    });

    return out;
}
