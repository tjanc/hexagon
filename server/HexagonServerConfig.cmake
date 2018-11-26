#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

include(CMakeFindDependencyMacro)
find_dependency(Catch2 1.0)
find_dependency(BoostBeast 1.66)
find_dependency(Boost 1.66)
find_dependency(Threads)
include("${CMAKE_CURRENT_LIST_DIR}/HexagonServerTargets.cmake")
