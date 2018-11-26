//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "font_cache.hpp"

using namespace hexagon::client;

font_cache::font_cache() : fonts_{sdl::font{"assets/font.ttf", 12}} {}

sdl::font& font_cache::at(int) { return fonts_.at(0); }
