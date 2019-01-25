//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_JOINED_BATTLE_MESSAGE_H_
#define HEXAGON_PROTOCOL_JOINED_BATTLE_MESSAGE_H_

#include <iostream>
#include <utility>

#include <hexagon/model/map.hpp>
#include <hexagon/model/team.hpp>

namespace hexagon::protocol
{
    static constexpr const char* JOINED_BATTLE_MESSAGE_ID = "JOINED_BATTLE";

    struct joined_battle_message {
        using placement_container =
            std::vector<std::pair<std::size_t, model::basic_map_index>>;

        model::team team;
        placement_container placements;

        joined_battle_message() = default;
        joined_battle_message(model::team t, placement_container p) noexcept;
    };

    std::istream& operator>>(std::istream& in, joined_battle_message& msg);
    std::ostream& operator<<(std::ostream& out,
                             const joined_battle_message& msg);
}  // namespace hexagon::protocol

#endif
