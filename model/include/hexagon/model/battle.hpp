//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_BATTLE_H_
#define HEXAGON_MODEL_BATTLE_H_

#include <utility>
#include <vector>

#include "map.hpp"
#include "team.hpp"

namespace hexagon::model
{
    using battle_id = std::size_t;

    class battle
    {
       public:
        using team_container = std::vector<team>;

       private:
        map map_ = {};
        team_container teams_ = {};
        int max_units_ = 1;

       public:
        battle() = default;
        explicit battle(map);
        battle(map, team_container);

        battle(const battle&) = default;
        battle(battle&&) noexcept = default;

        battle& operator=(const battle&) = default;
        battle& operator=(battle&&) noexcept = default;

        ~battle() = default;

       public:
        std::size_t join(const team&);
        team leave(int tid);

        team_container& teams() noexcept;
        const team_container& teams() const noexcept;

       public:
        map& get_map() noexcept;
        const map& get_map() const noexcept;
    };

}  // namespace hexagon::model

#endif
