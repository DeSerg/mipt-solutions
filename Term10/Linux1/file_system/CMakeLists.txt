cmake_minimum_required(VERSION 3.9)
project(file_system C)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

set(CMAKE_C_STANDARD 99)

set(FILE_SYSTEM
        file_system/mini_fs.c
        file_system/files.c
        file_system/superblock.c
        file_system/inode.c
)

set(TOOLS
        tools/tools.c
        tools/messages.c
)

set(SOURCES
        system.c
        ${FILE_SYSTEM}
        ${TOOLS}
)

add_executable(file_system_server server.c ${SOURCES})
add_executable(file_system_client client.c ${SOURCES} server.h)