//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_WORLD_CONTROLLER_H_
#define HEXAGON_CLIENT_WORLD_CONTROLLER_H_

#include <hexagon/model/world.hpp>
#include <hexagon/protocol/battle_message.hpp>
#include "world_facet.hpp"

namespace hexagon::client
{
    class mouse;
    class graphics;
    class game_controller;
}  // namespace hexagon::client

namespace hexagon::client
{
    class world_controller
    {
        model::world model_;
        world_facet facet_;

       public:
        world_controller(model::world, world_facet facet) noexcept;

        world_controller(const world_controller&) noexcept = delete;
        world_controller(world_controller&&) noexcept = default;
        world_controller& operator=(const world_controller&) noexcept = delete;
        world_controller& operator=(world_controller&&) noexcept = default;

       public:
        void update(game_controller&, const mouse&) noexcept;
        void update(game_controller&, protocol::battle_message);
        world_facet& facet() noexcept;

       public:
        void draw(graphics&) const;
    };
}  // namespace hexagon::client

#endif
