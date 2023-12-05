cmake_minimum_required(VERSION 3.23.0)

project(aoc2023)

# ==================================================================================================================== #
# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ==================================================================================================================== #
# Set compiler flags
set(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++20 -O3 -Wall -Wunused-variable -Wno-unknown-pragmas -g -fPIC -ffast-math")

# ==================================================================================================================== #
# Set the location of the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# ==================================================================================================================== #
# Set the include directories
INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/inc
 
  /usr/local/include
  /usr/include
)

# ==================================================================================================================== #
# C++ sources
message(STATUS ">>> Adding src subdirectory")
add_subdirectory(src)

# ==================================================================================================================== #
# Make the executable
add_executable(aoc2023 main.cpp ${AOC_SRCS})