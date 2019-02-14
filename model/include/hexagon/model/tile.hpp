//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_TILE_H_
#define HEXAGON_MODEL_TILE_H_

#include <cstdint>
#include <memory>
#include <variant>

namespace hexagon::model
{
    class unit;
}

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
        tile() noexcept = default;
        explicit tile(tile_type type, int elev = 0,
                      bool spawn = false) noexcept;

       public:
        bool is_spawn() const noexcept;
        bool is_spawnable() const noexcept;

        tile_type type() const noexcept;
        int elevation() const noexcept;

        unit* get_if_unit() noexcept;
        const unit* get_if_unit() const noexcept;

        bool has_unit() const noexcept;

        bool has_unit(const unit& u) const noexcept;
        bool has_unit(std::size_t uid) const noexcept;

        bool empty() const noexcept;
        void attach(unit& u);
        unit* detach_unit() noexcept;

        bool is_move_pending() const noexcept;
        void set_move_pending() noexcept;
    };

}  // namespace hexagon::model

#endif
