//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_BATTLE_MESSAGE_H_
#define HEXAGON_PROTOCOL_BATTLE_MESSAGE_H_

#include <iostream>

#include <hexagon/model/battle.hpp>

namespace hexagon::protocol
{
    static constexpr const char* BATTLE_MESSAGE_ID = "BATTLE";

    struct battle_message {
        hexagon::model::battle battle;
        battle_message() = default;
    };

    std::istream& operator>>(std::istream& in, battle_message& msg);
    std::ostream& operator<<(std::ostream& out, const battle_message& msg);
}  // namespace hexagon::protocol

#endif
