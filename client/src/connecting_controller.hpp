//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_CONNECTING_CONTROLLER_H_
#define HEXAGON_CLIENT_CONNECTING_CONTROLLER_H_

#include <hexagon/model/connecting.hpp>
#include <hexagon/protocol/login_response.hpp>
#include <hexagon/protocol/version_message.hpp>
#include "connecting_facet.hpp"

namespace hexagon::client
{
    class mouse;
    class graphics;
    class game_controller;
}  // namespace hexagon::client

namespace hexagon::client
{
    class connecting_controller
    {
        model::connecting model_;
        connecting_facet facet_;

       public:
        connecting_controller(connecting_facet facet) noexcept;

        connecting_controller(const connecting_controller&) noexcept = delete;
        connecting_controller(connecting_controller&&) noexcept = default;
        connecting_controller& operator=(
            const connecting_controller&) noexcept = delete;
        connecting_controller& operator=(connecting_controller&&) noexcept =
            default;

       public:
        void update(game_controller&, const mouse&) noexcept;
        void update(game_controller&, protocol::version_response);
        void update(game_controller&, protocol::login_response);
        connecting_facet& facet() noexcept;

       public:
        void draw(graphics&) const;
    };
}  // namespace hexagon::client

#endif
