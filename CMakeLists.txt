#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
set(CMAKE_CXX17_COMPILE_FEATURES "cxx_std_17")

set(CMAKE_PREFIX_PATH "/prefix")
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)

set(as_subproject
    HexagonModel
    HexagonClient
    HexagonServer
    Catch2
    BoostBeast)

macro(find_package)
    if(NOT "${ARGV0}" IN_LIST as_subproject)
        _find_package(${ARGV})
    endif()
endmacro()

# dependencies
if(NOT EMSCRIPTEN)
    add_subdirectory(ext/boost-beast EXCLUDE_FROM_ALL)
    add_library(boost-beast INTERFACE)
    target_include_directories(boost-beast INTERFACE ext/boost-beast/include)
    add_library(Boost::beast ALIAS boost-beast)
endif()


# testing dependencies
add_subdirectory(ext/Catch2 EXCLUDE_FROM_ALL)
include(CTest)
include(ext/Catch2/contrib/Catch.cmake)
enable_testing()

# targets developed in this super project
add_subdirectory(model EXCLUDE_FROM_ALL)
if(EMSCRIPTEN)
    add_subdirectory(client)
else()
    add_subdirectory(server)
endif()

