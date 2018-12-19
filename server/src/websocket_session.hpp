//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_WEBSOCKET_SESSION_H_
#define HEXAGON_SERVER_WEBSOCKET_SESSION_H_

#include <memory>
#include "local_state.hpp"
#include "net.hpp"

namespace hexagon::server
{
    class shared_state;

    class websocket_session
        : public std::enable_shared_from_this<websocket_session>
    {
        beast::flat_buffer buffer_;
        websocket::stream<tcp::socket> ws_;
        std::shared_ptr<shared_state> state_;
        local_state local_;
        std::vector<std::shared_ptr<std::string const>> queue_;

        void fail(boost::system::error_code ec, char const* what);
        void on_accept(boost::system::error_code ec);
        void on_read(boost::system::error_code ec, std::size_t);
        void on_write(boost::system::error_code ec, std::size_t);

       public:
        ~websocket_session();
        websocket_session(tcp::socket socket,
                          const std::shared_ptr<shared_state>& state);

       public:
        local_state& local() noexcept;
        const local_state& local() const noexcept;

       public:
        template <class Body, class Allocator>
        void run(http::request<Body, http::basic_fields<Allocator>> req);

        void send(std::shared_ptr<std::string const> const& ss);
    };

    template <class Body, class Allocator>
    void websocket_session::run(
        http::request<Body, http::basic_fields<Allocator>> req)
    {
        ws_.async_accept(
            req, [self = shared_from_this()](boost::system::error_code ec) {
                self->on_accept(ec);
            });
    }

}  // namespace hexagon::server

#endif
