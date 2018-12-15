//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/login_response.hpp>

#include <cassert>

namespace hexagon::protocol
{
    login_response::login_response(std::string k) noexcept : key{std::move(k)}
    {
    }

    std::istream& operator>>(std::istream& in, login_response& msg)
    {
        in >> msg.key;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const login_response& msg)
    {
        out << msg.key;

        return out;
    }
}  // namespace hexagon::protocol
