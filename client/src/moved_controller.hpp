//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MOVED_CONTROLLER_H_
#define HEXAGON_CLIENT_MOVED_CONTROLLER_H_

#include <vector>

#include <hexagon/model/unit_moved.hpp>

#include "battle_facet.hpp"
#include "moving_controller.hpp"

namespace hexagon::client
{
    class battle_controller;
    class mouse;
    class canvas;
}  // namespace hexagon::client

namespace hexagon::client
{
    class moved_controller
    {
        /// state of affairs
        model::unit_moved model_;

        /// projection
        battle_facet facet_;

       public:
        moved_controller(moving_controller) noexcept;

        moved_controller(const moved_controller&) noexcept = delete;
        moved_controller(moved_controller&&) noexcept = default;
        moved_controller& operator=(const moved_controller&) noexcept = delete;
        moved_controller& operator=(moved_controller&&) noexcept = default;

       public:
        void update(battle_controller&, const mouse&) noexcept;

       public:
        void draw(canvas&) const;

       public:
        const model::unit_moved& model() const noexcept;
        model::unit_moved& model() noexcept;
    };
}  // namespace hexagon::client

#endif
