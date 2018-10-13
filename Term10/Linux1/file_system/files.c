//
// Created by deserg on 13/10/2018.
//

#include "files.h"
#include <sys/stat.h>


int64_t file_size(int fd) {
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("files.c: file_size: failed to stat file descriptor");
        return -1;
    }

    return st.st_size;
}

int truncate_file(int fd, int64_t file_size) {
    if (ftruncate(fd, 0) == -1) {
        perror("files.c: truncate_file: failed to truncate to zero bytes");
        return -1;
    }
    if (ftruncate(fd, file_size) == -1) {
        fprintf(stderr, "files.c: truncate_file: failed to truncate to %lld bytes", file_size);
        return -1;
    }

    return 0;
}