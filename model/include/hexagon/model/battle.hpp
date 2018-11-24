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
        map::tile_container::iterator source;
        map::tile_container::iterator target;

        move_command(map::tile_container::iterator s,
                     map::tile_container::iterator t)
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
        map::tile_container::iterator source;
        map::tile_container::iterator target;
    };

}  // namespace hexagon::model

#endif
