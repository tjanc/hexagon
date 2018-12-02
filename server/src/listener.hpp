//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_LISTENER_H_
#define HEXAGON_SERVER_LISTENER_H_

#include <memory>
#include "net.hpp"

namespace hexagon::server
{
    class shared_state;

    class listener : public std::enable_shared_from_this<listener>
    {
        tcp::acceptor acceptor_;
        tcp::socket socket_;
        std::shared_ptr<shared_state> state_;

        void fail(boost::system::error_code ec, const char* what);
        void on_accept(boost::system::error_code ec);

       public:
        listener(net::io_context&, tcp::endpoint,
                 std::shared_ptr<shared_state>);
        void run();
    };

}  // namespace hexagon::server

#endif
