//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "font.hpp"

#include <SDL2/SDL_ttf.h>

using namespace hexagon::sdl;

void font_deleter::operator()(TTF_Font* font) { TTF_CloseFont(font); }

font::font(const char* file, int size) : raw_(TTF_OpenFont(file, size)) {}
