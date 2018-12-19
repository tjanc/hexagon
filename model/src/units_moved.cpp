//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/units_moved.hpp>

using namespace hexagon::model;

units_moved::units_moved(unit_moving prev) noexcept : team_{prev.my_team()} {}

const team& units_moved::my_team() const noexcept { return *team_; }

team& units_moved::my_team() noexcept { return *team_; }
