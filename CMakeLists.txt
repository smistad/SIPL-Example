cmake_minimum_required(VERSION 2.8)
project( SIPL-Example)

add_subdirectory(SIPL)
include_directories(${SIPL_INCLUDE_DIRS})
add_executable(example example.cpp)
target_link_libraries(example SIPL)
