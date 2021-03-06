//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_TEAM_H_
#define HEXAGON_MODEL_TEAM_H_

#include <vector>

#include "unit.hpp"

namespace hexagon::model
{
    struct team {
        using unit_container = std::vector<unit>;
        std::size_t id = 0;
        unit_container units;
        team() : id{0}, units{unit{}, unit{}} {}
        team(std::size_t which, unit_container us)
            : id{which}, units{std::move(us)}
        {
        }
        team(const team&) = default;
        team(team&&) noexcept = default;

        team& operator=(const team&) = default;
        team& operator=(team&&) noexcept = default;

        auto begin() const noexcept { return units.begin(); }
        auto begin() noexcept { return units.begin(); }
        auto end() const noexcept { return units.end(); }
        auto end() noexcept { return units.end(); }

        ~team() = default;
    };
}  // namespace hexagon::model

#endif
