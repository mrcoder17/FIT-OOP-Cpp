cmake_minimum_required(VERSION 3.17)
project(CircularBuffer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

add_subdirectory(lib/googletest-master)

include_directories(lib/googletest/include)
include_directories(lib/googlemock/include)

add_executable(CircularBuffer main.cpp CircularBuffer.h Tests.cpp)

target_link_libraries(CircularBuffer gtest gtest_main)
target_link_libraries(CircularBuffer gmock gmock_main)