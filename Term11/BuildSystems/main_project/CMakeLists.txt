cmake_minimum_required(VERSION 3.12)
project(main_project)

set(CMAKE_CXX_STANDARD 17)

# add the A library
add_subdirectory(A)

add_executable(main_project B/main.cpp)

target_include_directories(main_project PUBLIC
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/B")

target_link_libraries(main_project A)

