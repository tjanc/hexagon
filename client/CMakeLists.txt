#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(HexagonClient VERSION 0.1 LANGUAGES CXX)

find_package(HexagonModel 0.1 REQUIRED)

add_executable(client
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

if(EMSCRIPTEN)
    add_compile_options(-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s WASM=0)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(client PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --embed-file assets -s WASM=0 -s SDL2_IMAGE_FORMATS=[\"png\"]")
endif()

set_target_properties(client PROPERTIES OUTPUT_NAME hexagon-client)
target_compile_features(client PUBLIC cxx_std_17)

target_link_libraries(client
    PRIVATE
        hexagon::model
        ${SDL2_LIBRARIES}
    )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS client EXPORT client-targets
    RUNTIME DESTINATION bin
    )

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hexagon-client.js"
    DESTINATION
        bin
    )

