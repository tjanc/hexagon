//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/unit_moved.hpp>

using namespace hexagon::model;

unit_moved::unit_moved(battle b, std::size_t tidx) noexcept
    : model_{std::move(b)}, team_{std::next(model_.teams().begin(), tidx)}
{
}

unit_moved::unit_moved(battle b, battle::team_container::iterator tidx) noexcept
    : model_{std::move(b)}, team_{tidx}
{
}

const battle& unit_moved::battlefield() const noexcept { return model_; }

battle& unit_moved::battlefield() noexcept { return model_; }

const team& unit_moved::my_team() const noexcept { return *team_; }

team& unit_moved::my_team() noexcept { return *team_; }
