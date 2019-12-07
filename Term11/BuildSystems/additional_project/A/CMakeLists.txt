cmake_minimum_required(VERSION 3.12)
project(A)

set(CMAKE_CXX_STANDARD 17)

add_library(A lib.cpp)
target_include_directories(A PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

