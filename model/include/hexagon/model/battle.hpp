//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_BATTLE_H_
#define HEXAGON_MODEL_BATTLE_H_

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

       public:
        battle() = default;
        explicit battle(map);
        battle(map, team_container);

        battle(const battle&) = delete;
        battle(battle&&) noexcept = default;

        battle& operator=(const battle&) = delete;
        battle& operator=(battle&&) noexcept = default;

        ~battle() = default;

       public:
        team_container::iterator join(team);
        team leave(team_container::iterator);

        team_container& teams() noexcept;
        const team_container& teams() const noexcept;

       public:
        map& get_map() noexcept;
        const map& get_map() const noexcept;
    };

    struct move_command {
        basic_map_index source;
        basic_map_index target;

        move_command(basic_map_index s, basic_map_index t)
            : source(s), target(t)
        {
        }

        move_command() = default;
        move_command(const move_command&) = default;
        move_command(move_command&&) = default;
        move_command& operator=(const move_command&) = default;
        move_command& operator=(move_command&&) = default;
        ~move_command() = default;
    };

    struct attack_command {
        basic_map_index source;
        basic_map_index target;
    };

}  // namespace hexagon::model

#endif
