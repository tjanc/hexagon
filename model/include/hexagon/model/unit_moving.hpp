//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNIT_MOVING_H_
#define HEXAGON_MODEL_UNIT_MOVING_H_

#include <chrono>
#include <hexagon/model/battle.hpp>
#include <hexagon/model/map.hpp>
#include <hexagon/model/team.hpp>
#include <hexagon/model/units_joining.hpp>
#include <vector>

namespace hexagon::model
{
    struct movement {
        std::chrono::steady_clock::duration delay;
        basic_map_index source;
        basic_map_index target;

        movement() noexcept = default;
        movement(std::chrono::steady_clock::duration dt, basic_map_index src,
                 basic_map_index tgt) noexcept
            : delay(dt), source(src), target(tgt)
        {
        }

        bool operator<(const movement& rhs) const noexcept
        {
            return delay < rhs.delay;
        }
    };

    class unit_moving
    {
       public:
        using reach_map = basic_map<std::uint16_t>;
        using movement_container = std::vector<movement>;

       private:
        std::chrono::steady_clock::time_point start_;

        /// previous movements
        movement_container movements_;

        /// team under control
        team* team_;

        /// unit under control
        team::unit_container::iterator unit_;

        /// position unit on map
        basic_map_index unit_position_;

        /// reach of unit in control
        reach_map reach_map_;

       public:
        unit_moving(battle&, units_joining& prev) noexcept;

        unit_moving(battle&, team&, team::unit_container::iterator,
                    movement_container movements) noexcept;
        unit_moving(battle&, team&) noexcept;

        unit_moving(const unit_moving&) noexcept = delete;
        unit_moving(unit_moving&&) noexcept = default;
        unit_moving& operator=(const unit_moving&) noexcept = delete;
        unit_moving& operator=(unit_moving&&) noexcept = default;

       public:
        void move(basic_map_index idx);
        void next(battle& b);

       public:
        const team& my_team() const noexcept;
        team& my_team() noexcept;

        basic_map_index position() const noexcept;
        bool reachable(const map& m, basic_map_index idx) const noexcept;

        bool has_next() const noexcept;

        movement_container& movements() noexcept { return movements_; }
        const movement_container& movements() const noexcept
        {
            return movements_;
        }

        friend void move_unit(unit_moving&, basic_map_index) noexcept;
    };

}  // namespace hexagon::model

#endif
