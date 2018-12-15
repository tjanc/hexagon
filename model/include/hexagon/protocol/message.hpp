//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_MESSAGE_H_
#define HEXAGON_PROTOCOL_MESSAGE_H_

#include <sstream>
#include <string>
#include <variant>

#include "battle_message.hpp"
#include "login_request.hpp"
#include "login_response.hpp"
#include "map_request.hpp"
#include "map_response.hpp"
#include "unknown_message.hpp"
#include "version_message.hpp"

namespace hexagon::protocol
{
    template <typename>
    constexpr const char* id = "";

    template <>
    constexpr const char* id<version_response> = VERSION_RESPONSE_ID;

    template <>
    constexpr const char* id<map_request> = MAP_REQUEST_ID;

    template <>
    constexpr const char* id<map_response> = MAP_RESPONSE_ID;

    template <>
    constexpr const char* id<battle_message> = BATTLE_MESSAGE_ID;

    template <>
    constexpr const char* id<login_request> = LOGIN_REQUEST_ID;

    template <>
    constexpr const char* id<login_response> = LOGIN_RESPONSE_ID;

    using server_message = std::variant<  //
        unknown_message,                  //
        version_response,                 //
        map_response,                     //
        battle_message, login_response>;

    using client_message = std::variant<  //
        unknown_message,                  //
        map_request, login_request>;

    server_message read_server_message(const std::string& msg);
    client_message read_client_message(const std::string& msg);

    template <typename Message>
    std::string& write_message(std::string& buffer, const Message& m)
    {
        std::ostringstream ss(buffer);
        ss << id<Message> << ' ';
        ss << m;
        buffer = ss.str();
        return buffer;
    }

}  // namespace hexagon::protocol

#endif
