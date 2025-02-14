# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name and the C++ standard
project(MyProject CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the include directory (assuming the header file is in the 'include' directory)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the source files for the project
set(SOURCES
    src/elastic_hashing.cpp
    src/funnel_hashing.cpp
    test.cpp
)

# Add the executable target
add_executable(test ${SOURCES})

# Optionally, specify the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)