cmake_minimum_required(VERSION 3.10)
project(GeometryProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(triangles triangles.cpp)

add_executable(run_tests tests.cpp)

set_target_properties(triangles PROPERTIES
    DESCRIPTION "Main triangles program"
)

set_target_properties(run_tests PROPERTIES
    DESCRIPTION "Test suite for triangles"
)

message("=== Triangles Project ===")
message("To build: make")
message("To run main program: ./triangles")
message("To run tests: ./run_tests")