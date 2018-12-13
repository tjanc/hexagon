//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_LOGIN_RESPONSE_H_
#define HEXAGON_PROTOCOL_LOGIN_RESPONSE_H_

#include <hexagon/model/session.hpp>
#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* LOGIN_RESPONSE_ID = "LOGIN";

    struct login_response {
        model::session_key key;

        login_response() = default;
        explicit login_response(model::session_key) noexcept;
    };

    std::istream& operator>>(std::istream& in, login_response& msg);
    std::ostream& operator<<(std::ostream& out, const login_response& msg);
}  // namespace hexagon::protocol

#endif
