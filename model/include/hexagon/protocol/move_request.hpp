//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MOVE_REQUEST_H_
#define HEXAGON_PROTOCOL_MOVE_REQUEST_H_

#include <iostream>
#include <utility>
#include <vector>

#include <hexagon/model/map.hpp>

namespace hexagon::protocol
{
    static constexpr const char* MOVE_REQUEST_ID = "MOVEQ";

    struct move_request {
        using moves_container = std::vector<
            std::pair<model::basic_map_index, model::basic_map_index>>;
        moves_container moves;

        move_request() = default;
        explicit move_request(moves_container c) : moves{std::move(c)} {}
    };

    std::istream& operator>>(std::istream& in, move_request& msg);
    std::ostream& operator<<(std::ostream& out, const move_request& msg);
}  // namespace hexagon::protocol

#endif
