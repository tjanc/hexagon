//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_NET_H_
#define HEXAGON_NET_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;

#endif
