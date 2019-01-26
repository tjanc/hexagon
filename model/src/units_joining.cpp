//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/units_joining.hpp>

#include <hexagon/model/team.hpp>
#include <iostream>

using namespace hexagon::model;

units_joining::units_joining(team& t) noexcept : team_{&t} {}

const team& units_joining::my_team() const noexcept { return *team_; }

team& units_joining::my_team() noexcept { return *team_; }
