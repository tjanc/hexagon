//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_CONNECTION_H_
#define HEXAGON_CLIENT_CONNECTION_H_

#include <deque>
#include <string>
#include <vector>

#include <emscripten.h>

#include <hexagon/protocol/io/message_io.hpp>
#include <hexagon/protocol/message.hpp>

extern "C" {
EMSCRIPTEN_KEEPALIVE void* ws_alloc_buf(std::size_t n);
EMSCRIPTEN_KEEPALIVE void ws_message();
bool ws_send_js(const char*);
}

namespace
{
    using namespace hexagon::protocol::io;
}

namespace hexagon::client
{
    class connection
    {
        std::string input_buffer_;
        std::string output_buffer_;
        std::deque<hexagon::protocol::server_message> messages_;

       private:
        connection();

       public:
        static void open(const std::string& uri) {}

        static connection& instance()
        {
            static connection instance_;
            return instance_;
        }

        ~connection();

       public:
        template <typename Visitor>
        void handle_all(Visitor visit)
        {
            for (auto&& msg : messages_) visit(std::move(msg));
            messages_.clear();
        }

       public:
        template <typename Message, typename... Args>
        bool async_send(const Args&... args)
        {
            using namespace hexagon::protocol::io;
            hexagon::protocol::write_message<Message>(output_buffer_, args...);
            std::cout << "send (native): " << output_buffer_ << '\n';
            bool result = ws_send_js(output_buffer_.c_str());
            output_buffer_.clear();
            return result;
        }

       public:  // called internally
        void _message_buffered();
        void* _reset(std::size_t n);
    };  // namespace hexagon::client
}  // namespace hexagon::client

#endif
