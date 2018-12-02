//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_HTTP_SESSION_H_
#define HEXAGON_SERVER_HTTP_SESSION_H_

#include <memory>
#include "net.hpp"

namespace hexagon::server
{
    class shared_state;

    class http_session : public std::enable_shared_from_this<http_session>
    {
        tcp::socket socket_;
        beast::flat_buffer buffer_;
        std::shared_ptr<shared_state> state_;
        http::request<http::string_body> req_;

        void fail(boost::system::error_code ec, char const* what);
        void on_read(boost::system::error_code ec, std::size_t);
        void on_write(boost::system::error_code ec, std::size_t, bool close);

       public:
        http_session(tcp::socket socket,
                     const std::shared_ptr<shared_state>& state);
        void run();
    };

}  // namespace hexagon::server

#endif
