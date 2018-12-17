//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_EMRUN_H_
#define HEXAGON_EMRUN_H_

#include <emscripten.h>

extern "C" {
EMSCRIPTEN_KEEPALIVE int hexagon_emrun();
}

#endif
