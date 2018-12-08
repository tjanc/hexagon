//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "mouse.hpp"

#include <SDL.h>
#include <iostream>

using namespace hexagon::client;

namespace
{
    constexpr int PRESSED_FLAG = 0;
    constexpr int RELEASED_FLAG = 1;
    constexpr int DIRTY_FLAG = 2;
    constexpr int MOVED_FLAG = 3;
}  // namespace

mouse::mouse() noexcept : x(0), y(0) {}

mouse::mouse(std::uint16_t x_, std::uint16_t y_) noexcept : x(x_), y(y_) {}

void mouse::event(const SDL_MouseMotionEvent& e) noexcept
{
    if (!released()) {
        x = e.x;
        y = e.y;
        flags_.set(MOVED_FLAG);
        flags_.set(DIRTY_FLAG);
    }
}

void mouse::event(const SDL_MouseButtonEvent& e) noexcept
{
    x = e.x;
    y = e.y;

    if (e.state == SDL_PRESSED) {
        if (flags_.test(DIRTY_FLAG))
            std::cout << "WARN: pressed dirty\n";
        flags_.set(PRESSED_FLAG);
    } else if (e.state == SDL_RELEASED) {
        if (flags_.test(DIRTY_FLAG))
            std::cout << "WARN: released dirty\n";
        if (!flags_.test(PRESSED_FLAG)) {
            std::cout << "WARN: released without pressed\n";
        }
        flags_.reset(PRESSED_FLAG);
        flags_.set(RELEASED_FLAG);
    }
    flags_.set(DIRTY_FLAG);
}

bool mouse::released() const noexcept { return flags_.test(RELEASED_FLAG); }

bool mouse::moved() const noexcept { return flags_.test(MOVED_FLAG); }

bool mouse::dirty() const noexcept { return flags_.test(DIRTY_FLAG); }

void mouse::reset_dirty() noexcept
{
    flags_.reset(MOVED_FLAG);
    flags_.reset(RELEASED_FLAG);
    flags_.reset(DIRTY_FLAG);
}
