cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(FizzBuzzLib LANGUAGES CXX)

set(FIZZBUZZ_SOURCES
    src/fizzbuzz.cpp
    include/fizzbuzz.h)

add_library(fizzbuzz STATIC ${FIZZBUZZ_SOURCES})

target_include_directories(fizzbuzz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
