//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNIT_MOVED_H_
#define HEXAGON_MODEL_UNIT_MOVED_H_

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map.hpp>
#include <hexagon/model/team.hpp>

namespace hexagon::model
{
    class unit_moved
    {
       private:
        battle model_;
        battle::team_container::iterator team_;

       public:
        unit_moved(battle b, std::size_t tidx) noexcept;
        unit_moved(battle b, battle::team_container::iterator) noexcept;

        unit_moved(const unit_moved&) noexcept = delete;
        unit_moved(unit_moved&&) noexcept = default;
        unit_moved& operator=(const unit_moved&) noexcept = delete;
        unit_moved& operator=(unit_moved&&) noexcept = default;

       public:
        const battle& battlefield() const noexcept;
        battle& battlefield() noexcept;

        const team& my_team() const noexcept;
        team& my_team() noexcept;
    };

}  // namespace hexagon::model

#endif
