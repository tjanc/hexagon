#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(HexagonModel VERSION 0.0.2 LANGUAGES CXX)

add_library(model STATIC
    src/map_loader.cpp
    src/battle.cpp
    src/battle_io.cpp
    src/map.cpp
    src/map_io.cpp
    src/tile.cpp
    src/tile_io.cpp
    src/team_io.cpp
    src/unit.cpp
    src/unit_io.cpp
    src/units_joining.cpp
    src/unit_moving.cpp
    src/units_moved.cpp
    src/message.cpp
    src/version_message.cpp
    src/battle_message.cpp
    src/joined_battle_message.cpp
    src/login_request.cpp
    src/battle_request.cpp
    src/world_message.cpp
    src/move_request.cpp
    src/move_message.cpp
    src/local_state.cpp
    src/battling_state.cpp
    )
set_target_properties(model PROPERTIES OUTPUT_NAME hexagon-model)

target_compile_features(model PUBLIC cxx_std_17)

target_include_directories(model PUBLIC
    $<BUILD_INTERFACE:${HexagonModel_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${HexagonModel_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

install(TARGETS model EXPORT model-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
    )

install(EXPORT model-targets
    FILE HexagonModelTargets.cmake
    NAMESPACE hexagon::
    DESTINATION lib/cmake/hexagon
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file("HexagonModelConfigVersion.cmake"
    VERSION ${HexagonModel_VERSION}
    COMPATIBILITY SameMajorVersion
    )

install(
    FILES
        "HexagonModelConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/HexagonModelConfigVersion.cmake"
    DESTINATION
        lib/cmake/hexagon
    )

add_library(hexagon::model ALIAS model)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
