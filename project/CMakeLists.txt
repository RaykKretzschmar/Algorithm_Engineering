# Set the minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.22)

project(ImageProcessing)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_subdirectory(src)
add_subdirectory(catch)