cmake_minimum_required(VERSION 3.19)
project(project8 C)

set(CMAKE_C_STANDARD 11)

add_executable(project8 src/main.c src/cpu.h src/cpu.c)