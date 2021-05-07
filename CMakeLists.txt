cmake_minimum_required(VERSION 3.16)
project(project8 C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(project8 src/main.c src/cpu.h src/cpu.c)

target_link_libraries(project8 ${SDL2_LIBRARIES})