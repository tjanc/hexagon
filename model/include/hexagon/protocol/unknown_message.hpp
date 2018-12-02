//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_UNKNOWN_MESSAGE_H_
#define HEXAGON_PROTOCOL_UNKNOWN_MESSAGE_H_

#include <string>

namespace hexagon::protocol
{
    struct unknown_message {
        std::string id;
        std::string content;
        unknown_message(std::string id_) : id(id_) {}
    };
}  // namespace hexagon::protocol

#endif
