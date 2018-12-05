//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_MOUSE_H_
#define HEXAGON_CLIENT_MOUSE_H_

#include <bitset>
#include <cstdint>

struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

namespace hexagon::client
{
    class mouse {
        std::bitset<4> flags_;

       public:
        std::uint16_t x;
        std::uint16_t y;

       public:
        mouse() noexcept;
        mouse(std::uint16_t x, std::uint16_t y) noexcept;

       public:
        void event(const SDL_MouseMotionEvent& e) noexcept;
        void event(const SDL_MouseButtonEvent& e) noexcept;

       private:
        bool dirty() const noexcept;
        void reset_dirty() noexcept;

       public:
        template<typename Visitor>
        void handle_all(Visitor v) {
            if(dirty()) {
                v(*this);
                reset_dirty();
            }
        }

       public:
        bool clicked() const noexcept;
        bool released() const noexcept;
        bool moved() const noexcept;
    };
}  // namespace hexagon::client

#endif
