//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <catch2/catch.hpp>

#include <hexagon/model/map.hpp>

using namespace hexagon::model;

SCENARIO("Map model is created and tinkered with", "[json-value][null]")
{
    GIVEN("A list of tiles and a given width")
    {
        const auto tiles = map::tile_container{
            tile{tile_type::grass0}, tile{tile_type::grass1},  //
            tile{tile_type::dirt0},  tile{tile_type::dirt1},   //
            tile{tile_type::grass2},                           //
        };

        WHEN("from them a map is constructed")
        {
            map tested{tiles, 2};

            THEN("its size is the size of the list of tiles")
            {
                REQUIRE(tiles.size() == tested.size());
            }
        }
    }
}
