//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_MAP_ALGORITHM_H_
#define HEXAGON_MODEL_MAP_ALGORITHM_H_

#include <utility>
#include "map.hpp"
#include "path.hpp"
#include "unit.hpp"

namespace hexagon::model
{
    using reach_map = basic_map<std::uint16_t>;

    reach_map generate_reach_map(const map& m, basic_map_index center);

    std::pair<vertex_path::const_iterator, std::uint16_t> traversal_cost(
        const map& m, const unit& u, vertex_path::const_iterator source,
        vertex_path::const_iterator e);

    std::pair<vertex_path::const_iterator, std::uint16_t> traverse_unit(
        map& m, vertex_path::const_iterator source,
        vertex_path::const_iterator e);

    [[nodiscard]] unit* move_unit(map&, basic_map_index, basic_map_index,
                                  std::uint16_t stamina_cost = 0) noexcept;

}  // namespace hexagon::model

#endif
