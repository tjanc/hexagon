//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_SHARED_STATE_H_
#define HEXAGON_SERVER_SHARED_STATE_H_

#include <unordered_set>

#include <hexagon/model/battle.hpp>
#include "net.hpp"
#include "preload_assets.hpp"

namespace hexagon::state
{
    class world_state;
    class battling_state;
}  // namespace hexagon::state

namespace hexagon::server
{
    class websocket_session;

    class battle_lobby
    {
        model::battle battle_;
        std::unordered_set<websocket_session*> players_;

       public:
        explicit battle_lobby(model::battle b);

       public:
        model::team& join(websocket_session& ws, const state::world_state& s);
        void leave(websocket_session& ws, const state::battling_state& s);

       public:
        auto& battle() noexcept { return battle_; }
        const auto& battle() const noexcept { return battle_; }

        const auto& players() const noexcept { return players_; }
    };

    class shared_state
    {
        preload_assets assets_;
        battle_lobby lobby_;

        std::string document_root_;
        std::unordered_set<websocket_session*> sessions_;

       public:
        shared_state(preload_assets assets, std::string root);

        std::string version() const { return "0.0.2"; }

       public:
        void join(websocket_session&);
        void leave(websocket_session&);

       public:
        void handle(std::string, websocket_session& source);

       public:
        const std::string& document_root() const noexcept
        {
            return document_root_;
        }

       public:
        preload_assets& assets() noexcept { return assets_; }
        const preload_assets& assets() const noexcept { return assets_; }

        battle_lobby& lobby() noexcept { return lobby_; }
        const battle_lobby& lobby() const noexcept { return lobby_; }
    };

}  // namespace hexagon::server

#endif
