cmake_minimum_required(VERSION 3.9)
project(Chess_server)

set(CMAKE_CXX_STANDARD 11)

add_executable(Chess_server
        client.c
        Makefile
        messages.c
        messages.h
        README.md
        server.c
        server.h)
