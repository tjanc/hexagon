#  Copyright 2018 Thomas Jandecka.
#  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(HexagonServer VERSION 0.0.1 LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(BoostBeast 1.65 REQUIRED)
find_package(Boost 1.65 REQUIRED COMPONENTS system program_options)
find_package(Threads REQUIRED)

add_executable(server
    src/http_session.cpp
    src/listener.cpp
    src/shared_state.cpp
    src/websocket_session.cpp
    src/preload_assets.cpp
    src/main.cpp
    )
target_compile_definitions(server PRIVATE HEXAGON_SERVER_VERSION="${PROJECT_VERSION}")

target_link_libraries(server
    PRIVATE
        hexagon::model
        Boost::beast
        Threads::Threads
        ${Boost_LIBRARIES}
        stdc++fs
    )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(server PROPERTIES OUTPUT_NAME hexagon-server)

target_compile_features(server PUBLIC cxx_std_17)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/map2.map"
    DESTINATION lib/hexagon/assets
    )

install(TARGETS server EXPORT server-targets
    RUNTIME DESTINATION bin
    )

# Packaging support
set(CPACK_PACKAGE_VENDOR "Thomas Jandecka")
set(CPACK_PACKAGE_NAME "hexagon-server")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Some summary")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_INSTALL_CMAKE_PROJECTS
    ".;hexagon-server;ALL;."
    )

include(CPack)
