//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MOVE_REQUEST_H_
#define HEXAGON_PROTOCOL_MOVE_REQUEST_H_

#include <iostream>
#include <utility>
#include <vector>

#include <hexagon/model/map.hpp>
#include <hexagon/model/path.hpp>

namespace hexagon::protocol
{
    static constexpr const char* MOVE_REQUEST_ID = "MOVEQ";

    struct move_request {
        model::vertex_path route;

        move_request() = default;
        move_request(model::vertex_path p) : route(std::move(p)) {}
    };

    std::istream& operator>>(std::istream& in, move_request& msg);
    std::ostream& operator<<(std::ostream& out, const move_request& msg);
}  // namespace hexagon::protocol

#endif
