cmake_minimum_required(VERSION 3.9)
project(file_system C)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

set(CMAKE_C_STANDARD 99)

set(SOURCES
        inode.h
        system.h
        mini_fs.h)

add_executable(file_system main.c ${SOURCES})