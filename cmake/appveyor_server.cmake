cmake_minimum_required(VERSION 3.5)
set(CTEST_PROJECT_NAME "Hexagon")
set(CTEST_BUILD_NAME "Appveyor/Release")

set(CTEST_SOURCE_DIRECTORY ".")
set(CTEST_BINARY_DIRECTORY "build")

set(CTEST_USE_LAUNCHERS 1)

ctest_start("Continuous")
ctest_configure(OPTIONS)
ctest_build(TARGET server CONFIGURATION Release)
ctest_test()
ctest_submit()
