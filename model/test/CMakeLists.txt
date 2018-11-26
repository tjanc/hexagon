#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

find_package(HexagonModel 0.1 REQUIRED)
find_package(Catch2 1.0 REQUIRED)

add_executable(model-test
    runner.cpp
    test-map.cpp
    )

target_compile_features(model-test PUBLIC cxx_std_17)

set_target_properties(model-test PROPERTIES OUTPUT_NAME hexagon-model-test)

target_link_libraries(model-test
    PRIVATE
        hexagon::model
        Catch2::Catch2
    )

if(EMSCRIPTEN)
    find_program(Node node)
    add_compile_options(-s WASM=0)
    add_test(NAME HexagonModelTest COMMAND ${Node} "${CMAKE_CURRENT_BINARY_DIR}/hexagon-model-test.js")
else()
    catch_discover_tests(model-test)
endif()
