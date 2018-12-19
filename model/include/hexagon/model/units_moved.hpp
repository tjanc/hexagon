//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNITS_MOVED_H_
#define HEXAGON_MODEL_UNITS_MOVED_H_

#include "battle.hpp"
#include "team.hpp"
#include "unit_moving.hpp"

namespace hexagon::model
{
    class units_moved
    {
       private:
        battle::team_container::iterator team_;

       public:
        explicit units_moved(unit_moving) noexcept;

        units_moved(const units_moved&) noexcept = delete;
        units_moved(units_moved&&) noexcept = default;
        units_moved& operator=(const units_moved&) noexcept = delete;
        units_moved& operator=(units_moved&&) noexcept = default;

       public:
        const team& my_team() const noexcept;
        team& my_team() noexcept;
    };

}  // namespace hexagon::model

#endif
