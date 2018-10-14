//
// Created by deserg on 14/10/2018.
//

#ifndef FILE_SYSTEM_INODE_H
#define FILE_SYSTEM_INODE_H

#include <superblock.h>

// directories
bool directory_inodes(inode_t *directory, inode_t *directory_inodes);

bool directory_create(inode_t *parent, const char *directory_name);

bool directory_remove(inode_t *directory);

// files
bool file_contents(inode_t *file, int8_t *bytes);

bool file_create(inode_t *parent, const char *file_name, const int8_t *bytes);

bool file_remove(inode_t *file);

#endif //FILE_SYSTEM_INODE_H
