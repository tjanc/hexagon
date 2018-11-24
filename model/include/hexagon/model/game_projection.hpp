//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_GAME_PROJECTION_H_
#define HEXAGON_MODEL_GAME_PROJECTION_H_

#include <variant>

#include "battle_projection.hpp"
#include "world_projection.hpp"

namespace hexagon::model
{
    class game_projection
    {
        using state = std::variant<  //
            world_projection,        //
            battle_projection        //
            >;

       private:
        state state_;

       public:
        game_projection(battle&, team);

        template <typename Visitor>
        friend decltype(auto) visit(Visitor&& v, game_projection& self)
        {
            return std::visit(std::forward<Visitor>(v), self.state_);
        }
    };
}  // namespace hexagon::model

#endif
