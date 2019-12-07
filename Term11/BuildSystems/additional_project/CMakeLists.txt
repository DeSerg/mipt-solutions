cmake_minimum_required(VERSION 3.12)
project(additional_project)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(A)

add_executable(additional_project B/main.cpp)

target_include_directories(additional_project PUBLIC
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/B")

add_custom_target(
    index
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/C/preparing.py ${CMAKE_CURRENT_SOURCE_DIR}/C
)

add_custom_target(
    doxygen
    COMMAND doxygen doxygen.conf
)

add_dependencies(additional_project index doxygen)

target_link_libraries(additional_project A)

