cmake_minimum_required(VERSION 3.10)
project(reki CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(REKI_VENDORED "Use vendored libraries" ON)

if (REKI_VENDORED)
    add_subdirectory(lib/SDL3) # TODO: make this work on nix?
    add_subdirectory(lib/SDL3-ttf)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(SDL3_ttf REQUIRED CONFIG)
endif()

find_library(LIBCAP NAMES cap REQUIRED)

# embeds the font in the executable, etc.
add_custom_target(build_resources ALL
    COMMAND ${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/res
)

file(GLOB_RECURSE SRC src/*.cc)

add_executable(reki ${SRC})
add_dependencies(reki build_resources)

target_link_options(reki PRIVATE ${CMAKE_SOURCE_DIR}/res/font.o)
target_link_libraries(reki PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf ${LIBCAP})
