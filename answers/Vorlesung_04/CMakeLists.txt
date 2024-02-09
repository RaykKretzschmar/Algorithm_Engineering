cmake_minimum_required(VERSION 3.12)
project(QuickSortBenchmark)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find the OpenMP package
find_package(OpenMP REQUIRED)

# Add the executable
add_executable(QuickSortBenchmark min_max_quicksort_benchmark.cpp)

# Link OpenMP
target_link_libraries(QuickSortBenchmark PRIVATE OpenMP::OpenMP_CXX)



