add_executable(ImageProcessing main.cpp image_processing.cpp)

add_executable(Tests catch_main.cpp image_processing.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(ImageProcessing PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(Tests PUBLIC OpenMP::OpenMP_CXX)