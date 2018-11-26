//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/map_response.hpp>

#include <iomanip>
#include <iostream>

namespace hexagon::model
{
    std::ostream& operator<<(std::ostream& out, const tile_type& t)
    {
        switch (t) {
            case tile_type::grass0:
                out << 'g';
                return out;
            case tile_type::grass1:
                out << 'h';
                return out;
            case tile_type::grass2:
                out << 'k';
                return out;
            case tile_type::grass3:
                out << 'i';
                return out;
            case tile_type::grass4:
                out << 'j';
                return out;
            case tile_type::dirt0:
                out << 'd';
                return out;
            case tile_type::dirt1:
                out << 'e';
                return out;
            default:
                out << '-';
                return out;
        }
    }

    std::istream& operator>>(std::istream& in, tile_type& t)
    {
        char c = 0;
        in >> c;
        switch (c) {
            case 'g':
                t = tile_type::grass0;
                return in;
            case 'h':
                t = tile_type::grass1;
                return in;
            case 'k':
                t = tile_type::grass2;
                return in;
            case 'i':
                t = tile_type::grass3;
                return in;
            case 'j':
                t = tile_type::grass4;
                return in;
            case 'd':
                t = tile_type::dirt0;
                return in;
            case 'e':
                t = tile_type::dirt1;
                return in;
            default:
                return in;
        }
    }

    std::ostream& operator<<(std::ostream& out, const tile& t)
    {
        out << t.type() << ' '       //
            << t.elevation() << ' '  //
            << (t.is_spawn() ? '1' : '0');
        return out;
    }

    std::istream& operator>>(std::istream& in, tile& t)
    {
        tile_type type;
        in >> type;

        int elevation;
        in >> elevation;

        bool spawn;
        in >> spawn;

        t = tile{type, elevation, spawn};

        return in;
    }
}  // namespace hexagon::model

using namespace hexagon::model;

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, map_response& msg)
    {
        using hexagon::model::map;
        using hexagon::model::tile;

        int width = 0;
        in >> width;

        map::tile_container tiles;

        tile t;
        while (in >> t) {
            tile n;
            std::swap(t, n);
            tiles.emplace_back(std::move(n));
        }

        msg.map = map{std::move(tiles), width};

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const map_response& msg)
    {
        out << MAP_RESPONSE_ID << ' ';
        out << msg.map.width() << ' ';

        for (const auto& tile : msg.map) {
            out << tile;
        }

        return out;
    }
}  // namespace hexagon::protocol
