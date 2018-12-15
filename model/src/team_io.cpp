//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/team_io.hpp>

#include <cassert>
#include <ios>
#include <iostream>

#include <hexagon/protocol/io/unit_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

std::istream& io::operator>>(std::istream& in, team& obj)
{
    std::size_t id;
    in >> id;

    std::size_t n;
    in >> n;

    assert(n < 20);
    team::unit_container units;
    units.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        unit u;
        if (in >> u) units.emplace_back(std::move(u));
    }

    obj = team{id, std::move(units)};

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const team& obj)
{
    out << obj.id << ' '  //
        << obj.units.size() << ' ';
    for (const auto& u : obj.units) out << u;

    return out;
}
