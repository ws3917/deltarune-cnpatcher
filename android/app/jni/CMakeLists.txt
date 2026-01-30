cmake_minimum_required(VERSION 3.21)
project(cn_patcher)
find_package(SDL3 REQUIRED CONFIG)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(main SHARED ${SOURCES})
target_link_libraries(main PRIVATE SDL3::SDL3)
target_link_libraries(main PRIVATE android log)