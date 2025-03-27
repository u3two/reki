cmake_minimum_required(VERSION 3.10)
project(reki CXX)

# set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(REKI_VENDORED "Use vendored libraries" OFF)

if (REKI_VENDORED)
    add_subdirectory(lib/SDL3) # TODO: make this work on nix?
    add_subdirectory(lib/SDL3-ttf)
else()
    find_package(SQLite3 REQUIRED)
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(SDL3_ttf REQUIRED CONFIG)
endif()

add_subdirectory(lib/soci)


file(GLOB_RECURSE SRC src/*.cc)

add_executable(reki ${SRC})

target_link_libraries(reki PRIVATE soci_core soci_sqlite3 SDL3::SDL3 SDL3_ttf::SDL3_ttf)
