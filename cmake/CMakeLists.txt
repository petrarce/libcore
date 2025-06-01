project(libcore LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.16)
include(default.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(3rdparty)
add_subdirectory(core)
add_subdirectory(rendering/utils)
add_subdirectory(rendering/opengl)
