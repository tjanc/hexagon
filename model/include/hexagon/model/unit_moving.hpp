//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNIT_MOVING_H_
#define HEXAGON_MODEL_UNIT_MOVING_H_

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map.hpp>
#include <hexagon/model/team.hpp>
#include <vector>

namespace hexagon::model
{
    class unit_moving
    {
       public:
        using reach_map = basic_map<std::uint16_t>;
        using commands_container = std::vector<move_command>;

       private:
        /// battle unit is moving in
        battle model_;

        /// team in control
        battle::team_container::iterator team_;

        /// unit in control
        team::unit_container::iterator unit_;

        /// position unit on map
        basic_map_index unit_position_;

        /// reach of unit in control
        reach_map reach_map_;

        /// previous move commands
        std::vector<move_command> commands_;

       public:
        unit_moving(battle b, std::size_t tidx, std::size_t uidx,
                    commands_container cmds = {}) noexcept;
        unit_moving(battle b, std::size_t tidx) noexcept;

        unit_moving(const unit_moving&) noexcept = delete;
        unit_moving(unit_moving&&) noexcept = default;
        unit_moving& operator=(const unit_moving&) noexcept = delete;
        unit_moving& operator=(unit_moving&&) noexcept = default;

       public:
        void move(basic_map_index);
        void next();

       public:
        const battle& battlefield() const noexcept;
        battle& battlefield() noexcept;

        battle::team_container::const_iterator my_team() const noexcept;
        battle::team_container::iterator my_team() noexcept;

        basic_map_index position() const noexcept;

        bool reachable(basic_map_index) const noexcept;
        bool has_next() const noexcept;

        const commands_container& commands() const noexcept;
        commands_container& commands() noexcept;

        friend void move_unit(unit_moving&, basic_map_index) noexcept;
    };

}  // namespace hexagon::model

#endif
