//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/team_io.hpp>

#include <hexagon/model/team.hpp>

#include <cassert>
#include <ios>
#include <iostream>

#include <hexagon/protocol/io/message_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

std::istream& io::operator>>(std::istream& in, team& obj)
{
    std::size_t id;
    in >> id;

    team::unit_container units;
    in >> units;

    obj = team{id, std::move(units)};

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const team& obj)
{
    out << obj.id << ' ' << obj.units;

    return out;
}
