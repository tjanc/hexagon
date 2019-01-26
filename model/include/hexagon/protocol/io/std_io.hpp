//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_STD_IO_H_
#define HEXAGON_PROTOCOL_STD_IO_H_

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include <list>

namespace hexagon::protocol::io
{
    template <typename T, typename S>
    std::istream& operator>>(std::istream& in, std::pair<T, S>& obj)
    {
        in >> obj.first;
        in >> obj.second;
        return in;
    }

    template <typename T, typename S>
    std::ostream& operator<<(std::ostream& out, const std::pair<T, S>& obj)
    {
        out << obj.first           //
            << ' ' << obj.second;  //
        return out;
    }

    template <typename T>
    std::istream& operator>>(std::istream& in, std::vector<T>& obj)
    {
        std::size_t n;
        in >> n;

        obj.clear();
        obj.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            T t;
            if (in >> t) obj.emplace_back(std::move(t));
        }

        return in;
    }

    template <typename T>
    std::istream& operator>>(std::istream& in, std::list<T>& obj)
    {
        std::size_t n;
        in >> n;

        obj.clear();
        obj.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            T t;
            if (in >> t) obj.emplace_back(std::move(t));
        }

        return in;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& obj)
    {
        out << obj.size();
        for (const auto& t : obj) {
            out << ' ' << t;
        }
        return out;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const std::list<T>& obj)
    {
        out << obj.size();
        for (const auto& t : obj) {
            out << ' ' << t;
        }
        return out;
    }
}  // namespace hexagon::protocol::io

#endif
