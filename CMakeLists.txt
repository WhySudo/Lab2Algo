cmake_minimum_required(VERSION 3.23)
project(Lab2Algo)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab2Algo GoogleTests/ArrayTests.cpp Array.h)
add_subdirectory(GoogleTests)