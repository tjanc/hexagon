//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "websocket_session.hpp"

#include "shared_state.hpp"

using namespace hexagon::server;
using boost::system::error_code;

websocket_session::websocket_session(tcp::socket socket,
                                     const std::shared_ptr<shared_state>& state)
    : ws_(std::move(socket)), state_(state), local_{}
{
}

websocket_session::~websocket_session() { state_->leave(*this); }

void websocket_session::fail(error_code ec, char const* what)
{
    if (ec == net::error::operation_aborted || ec == websocket::error::closed)
        return;

    std::cerr << what << ": " << ec.message() << "\n";
}

void websocket_session::on_write(error_code ec, std::size_t)
{
    if (ec) return fail(ec, "write ws");

    queue_.erase(queue_.begin());

    if (!queue_.empty())
        ws_.async_write(
            net::buffer(*queue_.front()),
            [sp = shared_from_this()](error_code ec, std::size_t bytes) {
                sp->on_write(ec, bytes);
            });
}

void websocket_session::send(std::shared_ptr<std::string const> const& ss)
{
    queue_.push_back(ss);

    if (queue_.size() > 1) return;

    ws_.async_write(
        net::buffer(*queue_.front()),
        [sp = shared_from_this()](error_code ec, std::size_t bytes) {
            sp->on_write(ec, bytes);
        });
}

void websocket_session::on_read(error_code ec, std::size_t)
{
    if (ec) return fail(ec, "read ws");

    state_->handle(beast::buffers_to_string(buffer_.data()), *this);

    buffer_.consume(buffer_.size());

    ws_.async_read(buffer_,
                   [sp = shared_from_this()](error_code ec, std::size_t bytes) {
                       sp->on_read(ec, bytes);
                   });
}

void websocket_session::on_accept(error_code ec)
{
    if (ec) return fail(ec, "accept ws");

    state_->join(*this);

    ws_.async_read(buffer_,
                   [sp = shared_from_this()](error_code ec, std::size_t bytes) {
                       sp->on_read(ec, bytes);
                   });
}

local_state& websocket_session::local() noexcept { return local_; }

const local_state& websocket_session::local() const noexcept { return local_; }
