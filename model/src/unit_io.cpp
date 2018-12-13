//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/io/unit_io.hpp>

#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>

#include <hexagon/protocol/io/map_io.hpp>

using namespace hexagon::model;
using namespace hexagon::protocol;

std::istream& io::operator>>(std::istream& in, equipment& obj)
{
    in >> obj.name;

    std::size_t n;
    in >> n;

    assert(n < 10);
    equipment::powers_container es;
    es.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        power p;
        in >> p;
        es.emplace_back(std::move(p));
    }
    //
    return in;
}

std::ostream& io::operator<<(std::ostream& out, const equipment& obj)
{
    out << obj.name << ' ';

    out << obj.powers.size() << ' ';
    for (const auto& p : obj.powers) out << p;

    return out;
}

std::istream& io::operator>>(std::istream& in, power_class& obj)
{
    char c;
    in >> c;

    switch (c) {
        case 'w':
            obj = power_class::melee;
            break;
        case 'f':
            obj = power_class::fired;
            break;
        case 'm':
            obj = power_class::magic;
            break;
        default:
            obj = power_class::none;
            break;
    }

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const power_class& obj)
{
    switch (obj) {
        case power_class::melee:
            out << 'w';
            break;
        case power_class::fired:
            out << 'f';
            break;
        case power_class::magic:
            out << 'm';
            break;
        case power_class::none:
            out << '-';
            break;
        default:
            out << '-';
            assert(false);
            break;
    }

    return out;
}

std::istream& io::operator>>(std::istream& in, power& obj)
{
    using namespace hexagon::protocol::io;

    std::string name;
    in >> name >> std::ws;

    power_class klass;
    in >> klass;

    unit_job jtype;
    in >> jtype;

    std::uint16_t jp_req;
    in >> jp_req;

    std::uint16_t jp;
    in >> jp;

    damage_overlay overlay;
    in >> overlay;

    obj = power{name, jtype, klass, jp_req, jp, overlay};

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const power& obj)
{
    using namespace hexagon::protocol::io;

    out << obj.name() << ' '    //
        << obj.klass() << ' '   //
        << obj.type() << ' '    //
        << obj.jp_req() << ' '  //
        << obj.jp() << ' '      //
        << obj.overlay();       //

    return out;
}

std::istream& io::operator>>(std::istream& in, unit_job& obj)
{
    std::uint16_t u;
    in >> u;
    if (u > 18) {
        in.setstate(std::ios::failbit);
        return in;
    }
    obj = static_cast<unit_job>(static_cast<std::uint8_t>(u));
    return in;
}

std::ostream& io::operator<<(std::ostream& out, const unit_job& obj)
{
    out << static_cast<std::uint16_t>(obj);
    return out;
}

std::istream& io::operator>>(std::istream& in, unit_status& obj)
{
    in >> obj.health;
    in >> obj.stamina;
    in >> obj.magica;

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const unit_status& obj)
{
    out << obj.health << ' '   //
        << obj.stamina << ' '  //
        << obj.magica;         //

    return out;
}

std::istream& io::operator>>(std::istream& in, unit_statistics& obj)
{
    in >> obj.agility;
    in >> obj.presence;
    in >> obj.strength;
    in >> obj.intelligence;
    in >> obj.wisdom;
    in >> obj.endurance;

    return in;
}

std::ostream& io::operator<<(std::ostream& out, const unit_statistics& obj)
{
    out << obj.agility << ' '       //
        << obj.presence << ' '      //
        << obj.strength << ' '      //
        << obj.intelligence << ' '  //
        << obj.wisdom << ' '        //
        << obj.endurance;           //

    return out;
}

std::ostream& io::operator<<(std::ostream& out, const unit& obj)
{
    out << obj.id() << ' '          //
        << obj.job() << ' '         //
        << obj.level() << ' '       //
        << obj.experience() << ' '  //
        << obj.statistics() << ' '  //
        << obj.status() << ' '      //
        << obj.weapon() << ' ';     //

    out << obj.powers().size() << ' ';
    for (const auto& p : obj.powers()) out << p << ' ';

    return out;
}

std::istream& io::operator>>(std::istream& in, unit& t)
{
    std::size_t id;
    in >> id;

    unit_job job;
    in >> job;

    std::uint16_t level;
    in >> level;

    std::uint16_t exp;
    in >> exp;

    unit_statistics statistics;
    in >> statistics;

    unit_status status;
    in >> status;

    equipment weapon;
    in >> weapon;

    std::size_t n;
    in >> n;

    assert(n < 50);
    unit::powers_container powers;
    powers.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        power p;
        in >> p;
        powers.emplace_back(std::move(p));
    }

    t = unit{
        id,                 //
        job,                //
        level,              //
        exp,                //
        statistics,         //
        status,             //
        std::move(weapon),  //
        std::move(powers)   //
    };

    return in;
}
