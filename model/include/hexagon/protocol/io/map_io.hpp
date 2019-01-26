//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MAP_IO_H_
#define HEXAGON_PROTOCOL_MAP_IO_H_

#include <cassert>
#include <cstdint>
#include <iostream>

#include <hexagon/model/map.hpp>
#include "tile_io.hpp"

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream& in, model::basic_map_index& obj);
    std::ostream& operator<<(std::ostream& out,
                             const model::basic_map_index& obj);

    template <typename T>
    std::istream& operator>>(std::istream& in, model::basic_map<T>& obj)
    {
        using namespace hexagon::protocol::io;

        std::size_t n;
        in >> n;

        std::size_t width;
        in >> width;

        assert(n < 1000);
        typename model::basic_map<T>::tiles_container tiles;
        tiles.reserve(n);  // TODO transmit height as well?

        for (std::size_t i = 0; i < n; ++i) {
            T t;
            in >> t;
            tiles.emplace_back(std::move(t));
        }

        obj = model::basic_map<T>{std::move(tiles), width};

        return in;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const model::basic_map<T>& obj)
    {
        using namespace hexagon::protocol::io;

        out << obj.size() <<  //
            ' ' << obj.width();

        for (const auto& t : obj) out << ' ' << t;

        return out;
    }

    template <typename T>
    std::istream& operator>>(std::istream& in, model::basic_map_overlay<T>& obj)
    {
        model::basic_map_index center;
        in >> center;

        model::basic_map<T> overlay;
        in >> overlay;

        obj = model::basic_map_overlay<T>{std::move(overlay), center};

        return in;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out,
                             const model::basic_map_overlay<T>& obj)
    {
        out << obj.center() << ' ' << obj.overlay();
        return out;
    }
}  // namespace hexagon::protocol::io

#endif
