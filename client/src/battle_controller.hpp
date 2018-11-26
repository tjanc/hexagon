//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_BATTLE_CONTROLLER_H_
#define HEXAGON_CLIENT_BATTLE_CONTROLLER_H_

#include <variant>
#include <vector>

#include <hexagon/model/battle.hpp>

namespace hexagon::client
{
    struct move_state {
        std::vector<hexagon::model::move_command> commands_;
        hexagon::model::team::unit_container::iterator current_unit_;

        explicit move_state(hexagon::model::team& t)
            : commands_{}, current_unit_(t.units.begin())
        {
        }
    };

    struct move_state_committed {
    };

    struct attack_state {
        std::vector<hexagon::model::attack_command> commands_;
    };

    struct attack_state_committed {
    };

    class battle_controller
    {
        using state = std::variant<  //
            move_state,              //
            move_state_committed,    //
            attack_state,            //
            attack_state_committed   //
            >;

       private:
        hexagon::model::battle battle_;
        hexagon::model::battle::team_container::iterator team_;
        state state_;

       private:
        void mark_reachable();

       public:
        battle_controller(hexagon::model::battle b, hexagon::model::team t);

       public:  // commands
        void move(hexagon::model::map::tile_container::iterator target);

       public:  // access submodels
        hexagon::model::map& get_map() { return battle_.get_map(); }
        const hexagon::model::map& get_map() const { return battle_.get_map(); }
    };
}  // namespace hexagon::client

#endif
