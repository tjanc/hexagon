//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MESSAGE_H_
#define HEXAGON_PROTOCOL_MESSAGE_H_

#include <sstream>
#include <string>
#include <variant>

#include "battle_message.hpp"
#include "battle_request.hpp"
#include "login_request.hpp"
#include "move_message.hpp"
#include "move_request.hpp"
#include "unknown_message.hpp"
#include "version_message.hpp"
#include "world_message.hpp"

#include "io/message_io.hpp"

namespace hexagon::protocol
{
    template <typename>
    constexpr const char* id = "";

    template <>
    constexpr const char* id<version_response> = VERSION_RESPONSE_ID;

    template <>
    constexpr const char* id<battle_message> = BATTLE_MESSAGE_ID;

    template <>
    constexpr const char* id<login_request> = LOGIN_REQUEST_ID;

    template <>
    constexpr const char* id<battle_request> = BATTLE_REQUEST_ID;

    template <>
    constexpr const char* id<world_message> = WORLD_MESSAGE_ID;

    template <>
    constexpr const char* id<move_request> = MOVE_REQUEST_ID;

    template <>
    constexpr const char* id<move_message> = MOVE_MESSAGE_ID;

    using server_message = std::variant<  //
        unknown_message,                  //
        version_response,                 //
        battle_message,                   //
        world_message, move_message>;

    using client_message = std::variant<  //
        unknown_message,                  //
        login_request,                    //
        battle_request,                   //
        move_request>;

    server_message read_server_message(const std::string& msg);
    client_message read_client_message(const std::string& msg);

    template <typename Message, typename... Args>
    std::string& write_message(std::string& buffer, Args&&... args)
    {
        using namespace protocol::io;

        std::ostringstream ss(buffer);
        ss << id<Message>;

        auto list = {(!!(ss << ' ' << std::forward<Args>(args)))...};

        buffer = ss.str();
        return buffer;
    }

}  // namespace hexagon::protocol

#endif
