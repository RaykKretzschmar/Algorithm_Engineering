# Project

## Image Processing Program

The Image Processing Program is a sophisticated tool designed to enhance images through various processing techniques. It utilizes C++ and leverages OpenMP for parallel processing to optimize performance.
Unit tests are included to ensure the program's functionality and accuracy. For this Catch2 was used.

## Requirements

To compile and run this program, you need:

- CMake (version 3.22 or higher)
- A C++ compiler that supports C++11 or later
- OpenMP for parallel processing

## Compilation

Follow these steps to compile the program:

1. Create a `build/` directory inside the `project/` directory:

   ```bash
   mkdir -p project/build && cd project/build
   ```

2. Build the program using cmake

   ```bash
   cmake ..
   cmake --build .
   ```

3. Run the program src/ImageProcessing or do unit tests with src/Tests

## Execution

To run the program, navigate to the build/ directory and execute the program by specifying an image file and optional flags:

   ```bash
   cmake ..
   cmake --build .
   ```

### Possible Flags

 + __-primitive__: Utilizes a primitive threshold method
 + __-adaptive__: Utilizes the adaptive threshold method
 + __-proposed__: Utilizes a proposed threshold method
 + __-lmm__: Utilizes the local minimum maximum method

### Contributors

- **Robert Josef Domogalla** - Creator of the repository and contributor to the solutions.
- **Rayk Kretzschmar** - Collaborator on the project/paper.
- **Yassine Jabrane** - Collaborator on the project/paper.


Thank you for visiting this repository!