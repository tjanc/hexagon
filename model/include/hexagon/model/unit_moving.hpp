//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNIT_MOVING_H_
#define HEXAGON_MODEL_UNIT_MOVING_H_

#include <chrono>
#include <vector>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map.hpp>
#include <hexagon/model/map_algorithm.hpp>
#include <hexagon/model/path.hpp>
#include <hexagon/model/team.hpp>
#include <hexagon/model/units_joining.hpp>

namespace hexagon::model
{
    struct movement {
        std::chrono::steady_clock::duration delay;
        vertex_path route;

        movement() noexcept = default;
        movement(std::chrono::steady_clock::duration dt, vertex_path p) noexcept
            : delay(dt), route(std::move(p))
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
        bool follow(map& m, vertex_path route);
        bool end() const noexcept;
        void next(map& m) noexcept;

       public:
        const team& my_team() const noexcept;
        team& my_team() noexcept;

        basic_map_index position() const noexcept;
        const reach_map& reaches() const noexcept;
        [[deprecated]] bool reachable(const map& m, basic_map_index idx) const
            noexcept;

        movement_container& movements() noexcept { return movements_; }
        const movement_container& movements() const noexcept
        {
            return movements_;
        }
    };

}  // namespace hexagon::model

#endif
