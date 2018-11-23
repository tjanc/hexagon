#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(Hexagon VERSION 0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --embed-file assets -s WASM=0 -s SDL2_IMAGE_FORMATS=[\"png\"]")
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(hexagon
    src/model/map_loader.cpp

    src/model/battle.cpp
    src/model/map.cpp
    src/model/tile.cpp

    src/model/battle_projection.cpp
    src/model/game_projection.cpp

    src/sdl/renderer.cpp
    src/sdl/sdl.cpp
    src/sdl/surface.cpp
    src/sdl/texture.cpp
    src/sdl/window.cpp
    src/sdl/text.cpp

    src/game.cpp
    src/main.cpp
    src/map_facet.cpp
    src/map_textures.cpp
    src/battle_facet.cpp
    src/unit_facet.cpp
    )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(hexagon ${SDL2_LIBRARIES})
