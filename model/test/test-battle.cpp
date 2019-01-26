//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <catch2/catch.hpp>

#include <hexagon/model/battle.hpp>
#include <hexagon/model/map.hpp>

using namespace hexagon::model;

SCENARIO("Battle model is created and tinkered with", "[model][battle]")
{
    GIVEN("A map without spawn points")
    {
        const auto tiles = map::tiles_container{
            tile{tile_type::grass0}, tile{tile_type::grass1},  //
            tile{tile_type::dirt0},  tile{tile_type::dirt1},   //
            tile{tile_type::grass2},                           //
        };

        const map m{tiles, 2};

        WHEN("from it a battle with capacity 1 is constructed")
        {
            battle tested{m, 1};

            THEN("its teams are empty") { REQUIRE(0 == tested.teams().size()); }

            THEN("it is not ready") { REQUIRE(!tested.ready()); }

            THEN("it is full at 1") { REQUIRE(1 == tested.full()); }

            THEN("it's map matches the original in size")
            {
                REQUIRE(m.size() == tested.get_map().size());
            }

            THEN("it's map matches the original in width")
            {
                REQUIRE(m.width() == tested.get_map().width());
            }

            THEN("it's map matches the original in height")
            {
                REQUIRE(m.height() == tested.get_map().height());
            }

            GIVEN("A team and matching placements")
            {
                const team t{0, team::unit_container{
                                    unit{0, unit_job::warrior},  //
                                    unit{1, unit_job::mage}      //
                                }};

                const battle::placement_container ps{
                    basic_map_index{0, 0},  //
                    basic_map_index{1, 1},  //
                };

                WHEN("it is joined to the battle with placements")
                {
                    const team& joined_team = tested.join(t, ps);

                    THEN("battle teams size increased to 1")
                    {
                        REQUIRE(1 == tested.teams().size());
                    }

                    THEN("joined team size is leq the placements' size")
                    {
                        REQUIRE(ps.size() >= joined_team.units.size());
                    }

                    THEN("joined team size is leq the original team size")
                    {
                        REQUIRE(ps.size() >= t.units.size());
                    }

                    THEN("joined units are attached to tiles")
                    {
                        const auto& tested_map = tested.get_map();
                        for (const auto& u : joined_team.units) {
                            auto it = std::find_if(
                                tested_map.begin(), tested_map.end(),
                                [uptr = &u](const tile& tle) {
                                    return tle.get_if_unit() == uptr;
                                });
                            REQUIRE(tested_map.end() != it);
                        }
                    }

                    THEN("joined units are placed at currect positions")
                    {
                        const auto& tested_map = tested.get_map();
                        auto pit = ps.begin();
                        for (const auto& u : joined_team.units) {
                            auto it = std::find_if(
                                tested_map.begin(), tested_map.end(),
                                [uptr = &u](const tile& tle) {
                                    return tle.get_if_unit() == uptr;
                                });
                            REQUIRE(*pit == to_index(tested_map, it));
                            ++pit;
                        }
                    }

                    THEN("joined units are the only ones attached")
                    {
                        const auto& tested_map = tested.get_map();
                        const auto counted = std::count_if(
                            tested_map.begin(), tested_map.end(),
                            [](const tile& tle) { return tle.has_unit(); });

                        REQUIRE(joined_team.units.size() == counted);
                    }

                    THEN("the battle is full at 1")
                    {
                        REQUIRE(1 == tested.full());
                    }

                    THEN("the battle is ready")
                    {
                        REQUIRE(tested.ready());  //
                    }
                }
            }
        }
    }
}
