//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_PLAYER_H_
#define HEXAGON_MODEL_PLAYER_H_

#include "team.hpp"

namespace hexagon::model
{
    class player
    {
        std::string alias_;
        team team_;

       public:
        player() = default;
        player(std::string alias, team t)
            : team_(std::move(t)), alias_(std::move(alias))
        {
        }

        player(const player&) = default;
        player(player&&) noexcept = default;

        player& operator=(const player&) = default;
        player& operator=(player&&) noexcept = default;

        ~player() = default;

       public:
        const std::string& alias() const noexcept { return alias_; }
    };
}  // namespace hexagon::model

#endif
