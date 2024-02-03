cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(FizzBuzzProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(fizzbuzz)
add_subdirectory(catch)

add_executable(fizzbuzz_tests_exe catch_main.cpp catch_tests_fizzbuzz.cpp)

target_link_libraries(fizzbuzz_tests_exe PRIVATE fizzbuzz catch)