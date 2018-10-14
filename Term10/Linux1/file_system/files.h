//
// Created by deserg on 13/10/2018.
//

#ifndef FILE_SYSTEM_FILES_H
#define FILE_SYSTEM_FILES_H

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

bool set_fd_to_start();

int64_t file_size(int fd);

int truncate_file(int fd, int64_t file_size);

#endif //FILE_SYSTEM_FILES_H
