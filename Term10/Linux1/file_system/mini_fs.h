
#ifndef FILE_SYSTEM_MINI_FS_H
#define FILE_SYSTEM_MINI_FS_H

#include <inode.h>
#include <stdio.h>
#include <stdbool.h>


bool prepare_fs(const char *filename);


bool close_fs();


int ls(char *filepath);


int cat(FILE *fd, char *filepath);


int touch(FILE *fd, char *filepath, char *data);


int copy(FILE *fd, char *filepath_source, char *filepath_dest);


int move(FILE *fd, char *filepath_source, char *filepath_dest);


#endif //FILE_SYSTEM_MINI_FS_H