
#ifndef FILE_SYSTEM_MINI_FS_H
#define FILE_SYSTEM_MINI_FS_H

#include <superblock.h>
#include <stdio.h>
#include <stdbool.h>


bool prepare_fs(const char *filename);


bool close_fs();


bool ls(char *filepath);


bool mkdir(char *filepath);


int cat(char *filepath);


int touch(char *filepath, char *data);


int copy(char *filepath_source, char *filepath_dest);


int move(char *filepath_source, char *filepath_dest);


#endif //FILE_SYSTEM_MINI_FS_H
