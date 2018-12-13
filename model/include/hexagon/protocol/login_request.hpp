//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_LOGIN_REQUEST_H_
#define HEXAGON_PROTOCOL_LOGIN_REQUEST_H_

#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* LOGIN_REQUEST_ID = "LOGINQ";

    struct login_request {
        std::string name;
        std::string pw_hash;

        login_request() = default;
        explicit login_request(std::string);

        login_request(std::string, std::string);
    };

    std::istream& operator>>(std::istream& in, login_request& msg);
    std::ostream& operator<<(std::ostream& out, const login_request& msg);
}  // namespace hexagon::protocol

#endif
