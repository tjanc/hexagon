//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_SHARED_STATE_H_
#define HEXAGON_SERVER_SHARED_STATE_H_

#include <unordered_set>

#include <hexagon/model/battle.hpp>
#include "net.hpp"
#include "preload_assets.hpp"

namespace hexagon::server
{
    class websocket_session;

    class shared_state
    {
        preload_assets assets_;
        model::battle test_battle_;

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
    };

}  // namespace hexagon::server

#endif
