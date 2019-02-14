cmake_minimum_required(VERSION 3.5)
set(CTEST_PROJECT_NAME "Hexagon")
set(CTEST_BUILD_NAME "Unix/coverage")

set(CTEST_SOURCE_DIRECTORY ".")
set(CTEST_BINARY_DIRECTORY "build")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

set(CTEST_USE_LAUNCHERS 1)

set(CTEST_COVERAGE_COMMAND "/usr/bin/gcov")

set(Hexagon_CONFIG_OPTIONS
    "-DCMAKE_CXX_FLAGS='-Wall -Wpedantic -Wextra --coverage'"
    )

ctest_start("Continuous")
ctest_configure(OPTIONS "${Hexagon_CONFIG_OPTIONS}")
ctest_build(FLAGS -j5 TARGET model-test)
ctest_test()
ctest_coverage(CAPTURE_CMAKE_ERROR COVERAGE_ERROR)
ctest_submit()
