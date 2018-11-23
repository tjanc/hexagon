#include "font_cache.hpp"

using namespace hexagon;

font_cache::font_cache() : fonts_{sdl::font{"assets/font.ttf", 12}} {}

sdl::font& font_cache::at(int) { return fonts_.at(0); }
