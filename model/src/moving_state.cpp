//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/state/moving_state.hpp>

using namespace hexagon::state;
using namespace hexagon::model;

void moving_state::move(battle& b, basic_map_index idx)
{
    auto source = model_.position();
    move_unit(b.get_map(), source, idx);
    //responder_.unit_moved(source, idx);

    if (model_.has_next()) {
        model_.next(b);
    } else {
        //responder_.units_moved();
    }
}
