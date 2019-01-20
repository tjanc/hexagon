#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(HexagonClient VERSION 0.0.2 LANGUAGES CXX)

if(EMSCRIPTEN)
    find_package(HexagonModel 0.0.2 REQUIRED)
endif()

set(EMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES TRUE)

add_executable(client
    src/sdl/renderer.cpp
    src/sdl/sdl.cpp
    src/sdl/surface.cpp
    src/sdl/texture.cpp
    src/sdl/window.cpp
    src/sdl/text.cpp

    src/graphics.cpp
    src/mouse.cpp

    src/game.cpp
    src/emrun.cpp

    src/map_facet.cpp
    src/map_textures.cpp
    src/battle_facet.cpp
    src/unit_facet.cpp
    src/connecting_facet.cpp
    src/world_facet.cpp
    src/game_facet.cpp

    src/battling_adapter.cpp
    src/connecting_adapter.cpp
    src/world_adapter.cpp
    src/game_adapter.cpp

    src/connection.cpp
    )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/pre.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(client PRIVATE HEXAGON_CLIENT_VERSION="${PROJECT_VERSION}")

if(EMSCRIPTEN)
    add_compile_options(-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s WASM=0 -flto)
    set_target_properties(client PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s DISABLE_EXCEPTION_CATCHING=0 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --use-preload-plugins --preload-file assets -s WASM=0 -s SDL2_IMAGE_FORMATS=[\"png\"] -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"stringToUTF8\",\"lengthBytesUTF8\",\"writeStringToMemory\"] --pre-js pre.js -flto")
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
        "${CMAKE_CURRENT_SOURCE_DIR}/src/index.html"
        "${CMAKE_CURRENT_BINARY_DIR}/hexagon-client.data"
        "${CMAKE_CURRENT_BINARY_DIR}/hexagon-client.js.mem"
    DESTINATION
        bin
    )

install(
    FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/grass00.png"
    DESTINATION
        bin/assets
    )

if(CMAKE_BUILD_TYPE EQUAL Release)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hexagon-client.js.mem"
    DESTINATION
        bin
    )
endif()

# Packaging support
set(CPACK_PACKAGE_VENDOR "Thomas Jandecka")
set(CPACK_PACKAGE_NAME "hexagon")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Some summary")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_INSTALL_CMAKE_PROJECTS
    ".;hexagon;ALL;."
    )

include(CPack)
