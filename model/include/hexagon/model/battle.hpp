//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_BATTLE_H_
#define HEXAGON_MODEL_BATTLE_H_

#include <cassert>
#include <list>

#include "map.hpp"
#include "team.hpp"

namespace hexagon::model
{
    using battle_id = std::size_t;

    class battle
    {
       public:
        using team_container = std::list<team>;

       private:
        map map_;
        team_container teams_;

       public:
        explicit battle(map);

       public:
        team_container::iterator join(team);
        team leave(team_container::iterator);
        team_container& teams() { return teams_; }

       public:
        auto& get_map() { return map_; }
        const auto& get_map() const { return map_; }

       public:
        template <typename It>
        void commit_movements(It begin, It end)
        {
            for (; begin != end; ++begin) {
                unit* u = begin->source->detach_unit();
                assert(u);
                begin->target->attach(*u);
            }
        }
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
