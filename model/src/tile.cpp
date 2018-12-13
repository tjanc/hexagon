//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/tile.hpp>

using namespace hexagon::model;

namespace
{
    using tile_attrs = std::uint32_t;
    constexpr std::uint32_t tile_type_mask = 0xFFFF;
    constexpr std::uint32_t tile_spawn_mask = 0x10000;
}  // namespace

tile::tile(tile_type type, int elev, bool spawn) noexcept
    : attrs_{spawn ? (tile_spawn_mask | static_cast<std::uint16_t>(type))  //
                   : static_cast<std::uint16_t>(type)},
      elevation_{elev}
{
}

bool tile::is_spawn() const noexcept { return attrs_ & tile_spawn_mask; }
bool tile::is_spawnable() const noexcept { return is_spawn() && empty(); }

tile_type tile::type() const noexcept
{
    return tile_type(attrs_ & tile_type_mask);
}

int tile::elevation() const noexcept { return elevation_; }

bool tile::empty() const noexcept { return nullptr == object_; }

void tile::attach(unit& u) { object_ = &u; }

unit* tile::detach_unit() noexcept
{
    auto result = object_;
    object_ = nullptr;
    return result;
}

unit* tile::get_if_unit() noexcept { return object_; }
const unit* tile::get_if_unit() const noexcept { return object_; }

bool tile::has_unit() const noexcept { return nullptr != get_if_unit(); }

bool tile::has_unit(const unit& u) const noexcept
{
    const auto* u2 = get_if_unit();
    return u2 == &u;
}
