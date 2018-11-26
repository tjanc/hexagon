//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_VERSION_H_
#define HEXAGON_CLIENT_VERSION_H_

#ifndef HEXAGON_CLIENT_VERSION
#define HEXAGON_CLIENT_VERSION "experimental"
#endif

namespace hexagon
{
    constexpr const char* version() { return HEXAGON_CLIENT_VERSION; }
}  // namespace hexagon

#endif
