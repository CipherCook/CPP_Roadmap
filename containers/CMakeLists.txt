cmake_minimum_required(VERSION 3.20)
project(utk_containers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Catch2
find_package(Catch2 3 REQUIRED)

add_library(
    utk_containers
    utk_vector.cpp
)

add_executable(
    test_utk_vector 
    tests/test_utk_vector.cpp
)

target_link_libraries(
    test_utk_vector 
    PRIVATE 
    utk_containers 
    Catch2::Catch2WithMain
)

# Enable testing
enable_testing()
add_test(NAME utk_vector_tests COMMAND test_utk_vector)