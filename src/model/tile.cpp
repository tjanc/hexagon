//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "tile.hpp"

using namespace hexagon;

namespace
{
    using tile_attrs = std::uint32_t;
    constexpr std::uint32_t tile_type_mask = 0xFFFF;
    constexpr std::uint32_t tile_spawn_mask = 0x10000;
    constexpr std::uint32_t tile_reach_mask = 0x20000;
}  // namespace

tile::tile(tile_type type, int elev, bool spawn)
    : attrs_{spawn ? (tile_spawn_mask | static_cast<std::uint16_t>(type))  //
                   : static_cast<std::uint16_t>(type)},
      elevation_{elev}
{
}

bool tile::is_spawn() const { return attrs_ & tile_spawn_mask; }
bool tile::is_spawnable() const { return is_spawn() && empty(); }

bool tile::is_reachable() const { return attrs_ & tile_reach_mask; }
void tile::set_reachable() { attrs_ |= tile_reach_mask; }
void tile::reset_reachable() { attrs_ &= ~tile_reach_mask; }

tile_type tile::type() const { return tile_type(attrs_ & tile_type_mask); }

int tile::elevation() const { return elevation_; }

bool tile::empty() const { return nullptr == object_; }

void tile::attach(unit& u) { object_ = &u; }

unit* tile::detach_unit()
{
    auto result = object_;
    object_ = nullptr;
    return result;
}
