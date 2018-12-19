//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/message.hpp>

#include <sstream>

using namespace hexagon::protocol;

namespace
{
    template <typename Visitor>
    decltype(auto) visit_server_message(const std::string& msg, Visitor visit)
    {
        std::istringstream iss{msg};

        std::string name;
        iss >> name >> std::ws;

        if (name == id<version_response>) {
            version_response arg{};
            iss >> arg;
            return visit(std::move(arg));
        }

        if (name == id<battle_message>) {
            battle_message arg{};
            iss >> arg;
            return visit(std::move(arg));
        }

        if (name == id<login_response>) {
            login_response arg{};
            iss >> arg;
            return visit(std::move(arg));
        }

        {
            unknown_message arg{std::move(name)};
            iss >> arg.content;
            return visit(std::move(arg));
        }
    }

    template <typename Visitor>
    decltype(auto) visit_client_message(const std::string& msg, Visitor visit)
    {
        std::istringstream iss{msg};

        std::string name;
        iss >> name >> std::ws;

        if (name == id<login_request>) {
            login_request arg{};
            iss >> arg;
            return visit(std::move(arg));
        }

        if (name == id<move_request>) {
            move_request arg{};
            iss >> arg;
            return visit(std::move(arg));
        }

        {
            unknown_message arg{std::move(name)};
            iss >> arg.content;
            return visit(std::move(arg));
        }
    }
}  // namespace

server_message hexagon::protocol::read_server_message(const std::string& msg)
{
    return visit_server_message(msg, [](auto msg) {  //
        return server_message{std::move(msg)};
    });
}

client_message hexagon::protocol::read_client_message(const std::string& msg)
{
    return visit_client_message(msg, [](auto msg) {  //
        return client_message{std::move(msg)};
    });
}
