//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MOVING_CONTROLLER_H_
#define HEXAGON_CLIENT_MOVING_CONTROLLER_H_

#include <vector>

#include <hexagon/model/battle.hpp>

#include "battle_facet.hpp"

namespace hexagon::client
{
    class battle_controller;
    class mouse;
    class canvas;
}  // namespace hexagon::client

namespace hexagon::client
{
    class moving_controller
    {
        hexagon::model::battle model_;
        battle_facet facet_;

        hexagon::model::battle::team_container::iterator team_ptr_;
        hexagon::model::team::unit_container::iterator unit_ptr_;

        std::vector<hexagon::model::move_command> commands_;

       public:
        moving_controller(
            hexagon::model::battle b,
            hexagon::model::battle::team_container::iterator t,
            hexagon::model::team::unit_container::iterator u) noexcept;

        moving_controller(const moving_controller&) noexcept = delete;
        moving_controller(moving_controller&&) noexcept = default;
        moving_controller& operator=(const moving_controller&) noexcept =
            delete;
        moving_controller& operator=(moving_controller&&) noexcept = default;

       public:
        void update(battle_controller&, const mouse&) noexcept;

       public:
        void draw(canvas&) const;

       public:
        const hexagon::model::battle& model() const noexcept;
        hexagon::model::battle& model() noexcept;

        hexagon::model::team::unit_container::iterator unit() noexcept;
        hexagon::model::team::unit_container::const_iterator unit() const
            noexcept;
    };
}  // namespace hexagon::client

#endif
