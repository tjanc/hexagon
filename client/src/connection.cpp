//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "connection.hpp"

#include <hexagon/protocol/version_message.hpp>

// clang-format off
EM_JS(void, ws_connect_js, (), {
  console.log("connecting");
  web_socket_ = new WebSocket("ws://" + window.location.hostname + ":8080");
  web_socket_.binary_type = "arraybuffer";

  web_socket_.addEventListener('open', function(event) {
    ws_socket_opened_ = true;
    console.log("connected");
  });

  web_socket_.addEventListener('message', function(event) {
    if (typeof event.data === 'string' || event.data instanceof String) {
      console.log("read: `" + event.data + "`");
      const len = Module.lengthBytesUTF8(event.data) + 1;
      var buf = _ws_alloc_buf(len);
      Module.stringToUTF8(event.data, buf, len);
      _ws_message();
    } else {
      console.log("non-string message data: " + (typeof event.data));
    }
  });
});

EM_JS(void, ws_close_js, (), {
  if(ws_socket_opened_) {
    web_socket_.close();
  }
});

EM_JS(bool, ws_send_js, (const char* msg), {
  if(ws_socket_opened_) {
    const str = Pointer_stringify(msg);
    console.log("send: `" + str + "`");
    web_socket_.send(str);
    return true;
  }
  return false;
});
// clang-format on

using namespace hexagon::client;
using namespace hexagon::protocol;

void* connection::_reset(std::size_t n)
{
    input_buffer_.resize(n, '-');
    return input_buffer_.data();
}

void connection::_message_buffered()
{
    // ignore null terminator from JS
    // TODO: solve in inline JS before writing to buffer
    input_buffer_.pop_back();
    auto result = read_server_message(input_buffer_);
    input_buffer_.clear();

    messages_.emplace_back(std::move(result));
}

connection::connection() { ws_connect_js(); }

connection::~connection() { ws_close_js(); }

void* ws_alloc_buf(std::size_t len)
{
    return connection::instance()._reset(len);
}

void ws_message() { connection::instance()._message_buffered(); }
