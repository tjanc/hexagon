//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/login_request.hpp>

#include <cassert>
#include <string>

namespace hexagon::protocol
{
    login_request::login_request(std::string n) : name{std::move(n)}, pw_hash{}
    {
    }

    login_request::login_request(std::string n, std::string pw)
        : login_request(std::move(n))
    {
        assert(false);  // hashing not implemented
    }

    std::istream& operator>>(std::istream& in, login_request& msg)
    {
        in >> msg.name;
        in >> msg.pw_hash;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const login_request& msg)
    {
        out << msg.name << ' ' << msg.pw_hash;

        return out;
    }
}  // namespace hexagon::protocol
