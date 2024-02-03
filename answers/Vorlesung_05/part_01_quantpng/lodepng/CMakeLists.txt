cmake_minimum_required(VERSION 3.9)
project(lodepng LANGUAGES CXX)

set(LODEPNG_SOURCES
    lodepng.cpp
    lodepng.h)

add_library(lodepng STATIC ${LODEPNG_SOURCES})

target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
