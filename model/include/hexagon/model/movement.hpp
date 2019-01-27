//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_MOVEMENT_H_
#define HEXAGON_MODEL_MOVEMENT_H_

#include <chrono>
#include <cstdint>
#include <set>
#include "map.hpp"
#include "unit.hpp"

namespace hexagon::model
{
    enum class path_model { vertices };

    template <path_model>
    class path;

    template <>
    class path<path_model::vertices>
    {
       public:
        using value_type = basic_map_index;
        using container = std::deque<value_type>;
        using iterator = typename container::iterator;
        using const_iterator = typename container::const_iterator;

       private:
        container vertices_;

       public:
        path() noexcept = default;

       public:
        container& vertices() noexcept { return vertices_; }
        const container& vertices() const noexcept { return vertices_; }
    };

    using vertex_path = path<path_model::vertex>;

    class movement
    {
        int time_penalty_;
        vertex_path path_;

       public:
        movement() = default;
        movement(std::chrono::steady_clock::duration response_time,
                      basic_map_index source, basic_map_index target)
            : time_penalty_(
                  std::chrono::duration_cast<std::chrono::milliseconds>(
                      response_time)
                      .count()),
              source_(source),
              target_(target)
        {
        }

        unit_movement(const unit_movement&) = default;
        unit_movement(unit_movement&&) noexcept = default;

        unit_movement& operator=(const unit_movement&) = default;
        unit_movement& operator=(unit_movement&&) noexcept = default;

        ~unit_movement() = default;

       public:
        const basic_map_index& source() const noexcept { return source_; }
        const basic_map_index& target() const noexcept { return target_; }

        int penalty() const noexcept { return time_penalty_; }
    };

    class movement_buffer
    {
       public:
        struct by_penalty {
            template <typename T>
            bool operator()(const T& lhs, const T& rhs)
            {
                return lhs.penalty() < rhs.penalty();
            }
        };

       public:
        using movement_container = std::set<unit_movement, by_penalty>;

       private:
        movement_container movements_;

       public:
        movement_buffer() = default;

       public:
        template <typename... Args>
        void emplace(Args&&... args)
        {
            movements_.emplace(std::forward<Args>(args)...);
        }

       public:
        auto begin() const noexcept { return movements_.begin(); }
        auto end() const noexcept { return movements_.end(); }
    };

}  // namespace hexagon::model

#endif
