//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MOVING_CONTROLLER_H_
#define HEXAGON_CLIENT_MOVING_CONTROLLER_H_

#include <vector>

#include <hexagon/model/unit_moving.hpp>

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
        using reach_map = model::basic_map_overlay<std::uint16_t>;

       private:
        /// state of affairs
        model::unit_moving model_;

        /// projection
        battle_facet facet_;

        /// previous move commands
        std::vector<model::move_command> commands_;

       public:
        moving_controller(model::unit_moving model,  //
                          int width, int height) noexcept;

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
        const model::unit_moving& model() const noexcept;
        model::unit_moving& model() noexcept;
    };
}  // namespace hexagon::client

#endif
