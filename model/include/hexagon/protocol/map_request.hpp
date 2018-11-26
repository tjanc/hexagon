//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MAP_REQUEST_H_
#define HEXAGON_PROTOCOL_MAP_REQUEST_H_

#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* MAP_REQUEST_ID = "MAPQ";

    struct map_request {
        int id = 0;
        map_request() = default;
        explicit map_request(int i) : id(i) {}
    };

    std::istream& operator>>(std::istream& in, map_request& msg);
    std::ostream& operator<<(std::ostream& out, const map_request& msg);
}  // namespace hexagon::protocol

#endif
