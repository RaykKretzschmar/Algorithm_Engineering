cmake_minimum_required(VERSION 3.10)

project(quantpng)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lodepng)
add_subdirectory(quantize)

add_executable(quantpng_exe main.cpp)

target_link_libraries(quantpng_exe PRIVATE lodepng quantize)
