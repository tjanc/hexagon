//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_PATH_H_
#define HEXAGON_MODEL_PATH_H_

#include <deque>
#include "map.hpp"

namespace hexagon::model
{
    class vertex_path
    {
       public:
        using value_type = basic_map_index;
        using container = std::deque<value_type>;
        using iterator = typename container::iterator;
        using const_iterator = typename container::const_iterator;

       private:
        container vertices_;

       public:
        vertex_path() = default;

       public:
        container& vertices() noexcept { return vertices_; }
        const container& vertices() const noexcept { return vertices_; }

       public:
        const_iterator source() const noexcept { return vertices_.begin(); }
        const_iterator target() const noexcept
        {
            return vertices_.begin() + (vertices_.size() - 1);
        }
        const_iterator end() const noexcept { return vertices_.end(); }
    };

}  // namespace hexagon::model

#endif
