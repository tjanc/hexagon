//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_BATTLE_H_
#define HEXAGON_MODEL_BATTLE_H_

#include <list>
#include <utility>

#include "map.hpp"
#include "team.hpp"

namespace hexagon::model
{
    using battle_id = std::size_t;

    class battle
    {
       public:
        using team_container = std::list<team>;
        using placement_container =
            std::vector<std::pair<std::size_t, basic_map_index>>;

       private:
        map map_ = {};
        team_container teams_ = {};
        int max_units_ = 1;
        std::size_t full_teams_ = 2;

       public:
        battle() = default;
        explicit battle(map, std::size_t full);
        battle(map, std::size_t full, team_container);

        battle(const battle&);
        battle(battle&&) noexcept = default;

        battle& operator=(const battle&);
        battle& operator=(battle&&) noexcept = default;

        ~battle() = default;

       public:
        std::pair<team*, placement_container> join(const team&);
        team leave(int tid);

        team_container& teams() noexcept;
        const team_container& teams() const noexcept;

       public:
        map& get_map() noexcept;
        const map& get_map() const noexcept;

       public:
        bool ready() const noexcept;
        std::size_t full() const noexcept;
    };

}  // namespace hexagon::model

#endif
