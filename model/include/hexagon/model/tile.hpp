//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_TILE_H_
#define HEXAGON_MODEL_TILE_H_

#include <cstdint>
#include <memory>
#include <variant>
#include "unit.hpp"

namespace hexagon::model
{
    enum class tile_type : std::uint16_t {
        grass0 = 0,
        grass1,
        grass2,
        grass3,
        grass4,
        dirt0,
        dirt1
    };

    class tile
    {
        std::uint32_t attrs_ = static_cast<std::uint16_t>(tile_type::grass0);
        int elevation_ = 0;
        unit* object_ = nullptr;

       public:
        tile() = default;
        tile(tile_type type, int elev = 0, bool spawn = false);

       public:
        bool is_spawn() const;
        bool is_spawnable() const;
        bool is_reachable() const;
        void set_reachable();
        void reset_reachable();

        tile_type type() const;
        int elevation() const;

        unit* get_if_unit() { return object_; }
        const unit* get_if_unit() const { return object_; }

        bool has_unit(const unit& u) const
        {
            const auto* u2 = get_if_unit();
            return u2 == &u;
        }

        bool empty() const;
        void attach(unit& u);
        unit* detach_unit();
    };

}  // namespace hexagon

#endif
