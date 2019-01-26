cmake_minimum_required(VERSION 3.5)
set(CTEST_PROJECT_NAME "Hexagon")
if("${CTEST_BUILD_NAME}" STREQUAL "")
    set(CTEST_BUILD_NAME "Unix")
else()
    set(CTEST_BUILD_NAME "Unix${CTEST_BUILD_NAME}")
endif()

set(CTEST_SOURCE_DIRECTORY ".")
set(CTEST_BINARY_DIRECTORY "build")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_MEMORYCHECK_TYPE UndefinedBehaviorSanitizer)
set(CTEST_MEMORYCHECK_SANITIZER_OPTIONS "print_stacktrace=1:halt_on_error=1")

set(CTEST_USE_LAUNCHERS 1)

set(Hexagon_CONFIG_OPTIONS
    "-DCMAKE_CXX_FLAGS='-Wall -Wpedantic -Wextra'"
    "-DCMAKE_CXX_FLAGS='-fsanitize=undefined'"
    )

ctest_start("Continuous")
ctest_configure(OPTIONS "${Hexagon_CONFIG_OPTIONS}")
ctest_build(FLAGS -j5 TARGET model-test)
ctest_build(FLAGS -j5 TARGET server)
ctest_test()
ctest_memcheck(EXCLUDE DrafterIntegration)
ctest_submit()
