//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_BATTLE_PROJECTION_H_
#define HEXAGON_MODEL_BATTLE_PROJECTION_H_

#include <variant>
#include <vector>

#include "battle.hpp"

namespace hexagon::model
{
    struct move_state {
        std::vector<move_command> commands_;
        team::unit_container::iterator current_unit_;

        explicit move_state(team& t)
            : commands_{}, current_unit_(t.units.begin())
        {
        }
    };

    struct move_state_committed {
    };

    struct attack_state {
        std::vector<attack_command> commands_;
    };

    struct attack_state_committed {
    };

    class battle_projection
    {
        using state = std::variant<  //
            move_state,              //
            move_state_committed,    //
            attack_state,            //
            attack_state_committed   //
            >;

       private:
        battle* battle_;
        battle::team_container::iterator team_;
        state state_;

       private:
        void mark_reachable();

       public:
        battle_projection(battle& b, team t);

       public:  // commands
        void move(map::tile_container::iterator target);

       public:  // access submodels
        map& get_map() { return battle_->get_map(); }
        const map& get_map() const { return battle_->get_map(); }
    };
}  // namespace hexagon::model

#endif
