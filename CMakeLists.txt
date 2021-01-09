cmake_minimum_required(VERSION 3.16)
project(red_course)

set(CMAKE_CXX_STANDARD 20)

add_compile_options( -W )
add_compile_options( -Wall )
add_compile_options( -Werror )
add_compile_options( -Wpedantic )

add_executable(red_course swap_sort_copy.cpp)