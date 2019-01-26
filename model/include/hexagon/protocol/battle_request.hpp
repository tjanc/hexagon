//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_BATTLE_REQUEST_H_
#define HEXAGON_PROTOCOL_BATTLE_REQUEST_H_

#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* BATTLE_REQUEST_ID = "BATTLEQ";

    struct battle_request {
        int id = 0;

        battle_request() noexcept = default;
        battle_request(int i) noexcept : id{i} {}
    };

    std::istream& operator>>(std::istream& in, battle_request& msg);
    std::ostream& operator<<(std::ostream& out, const battle_request& msg);
}  // namespace hexagon::protocol

#endif
