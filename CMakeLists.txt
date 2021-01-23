cmake_minimum_required(VERSION 3.16)
project(red_course)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_BUILD_TYPE Release)
add_compile_options( -W )
add_compile_options( -Wall )
#add_compile_options( -Werror )
add_compile_options( -Wpedantic )

add_executable(${PROJECT_NAME} synchronized.cpp)

target_link_libraries(
          ${PROJECT_NAME}
          pthread
)
