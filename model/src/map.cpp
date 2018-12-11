//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/map.hpp>
#include <hexagon/model/unit.hpp>

#include <algorithm>
#include <iostream>

using namespace hexagon::model;
namespace
{
    template <typename It>
    It find_spawnable(It begin, It end)
    {
        return std::find_if(begin, end, [](const tile& t) {  //
            return t.is_spawnable();
        });
    }
}  // namespace

map::iterator hexagon::model::spawn(map& m, unit& u)
{
    using std::begin;
    using std::end;

    auto it = find_spawnable(m.begin(), m.end());
    if (it == m.end()) {
        std::cout << "no spawn point found\n";
        return it;
    }
    std::cout << "spawn point found\n";
    it->attach(u);
    return it;
}

basic_map_index hexagon::model::find_unit(const map& m, const unit& u) noexcept
{
    auto it = std::find_if(m.begin(), m.end(),
                           [&u](const auto& tile) { return tile.has_unit(u); });

    if (m.end() == it) return basic_map_index{};
    return to_index(m, it);
}

bool hexagon::model::operator==(const basic_map_index& lhs,
                                const basic_map_index& rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

basic_map_index hexagon::model::operator+(const basic_map_index& lhs,
                                          const basic_map_index& rhs) noexcept
{
    return basic_map_index{lhs.x + rhs.x, lhs.y + rhs.y};
}

basic_map_index hexagon::model::operator-(const basic_map_index& lhs,
                                          const basic_map_index& rhs) noexcept
{
    return basic_map_index{lhs.x - rhs.x, lhs.y - rhs.y};
}

basic_map_index hexagon::model::west(basic_map_index i) noexcept
{
    --i.x;
    return i;
}

basic_map_index hexagon::model::north_west(basic_map_index i) noexcept
{
    --i.y;
    i.x -= i.y % 2;
    return i;
}

basic_map_index hexagon::model::north_east(basic_map_index i) noexcept
{
    auto result = north_west(i);
    ++result.x;
    return result;
}

basic_map_index hexagon::model::east(basic_map_index i) noexcept
{
    ++i.x;
    return i;
}

basic_map_index hexagon::model::south_east(basic_map_index i) noexcept
{
    ++i.y;
    i.x += (i.y - 1) % 2;
    return i;
}

basic_map_index hexagon::model::south_west(basic_map_index i) noexcept
{
    auto result = south_east(i);
    --result.x;
    return result;
}
