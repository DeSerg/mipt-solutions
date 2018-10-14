//
// Created by deserg on 14/10/2018.
//

#ifndef FILE_SYSTEM_INODE_H
#define FILE_SYSTEM_INODE_H

#include <superblock.h>

// general
bool get_free_inode(inode_t **inode);

bool read_inode_data(inode_t *directory, uint8_t *bytes);

// directories
bool directory_inodes(inode_t *directory, int32_t *inodes_count, inode_t **inodes, char ***filenames);

bool directory_create(inode_t *parent, const char *directory_name);

bool directory_remove(inode_t *directory);

// files
bool file_contents(inode_t *file, int32_t *bytes_num, uint8_t **bytes);

bool file_create(inode_t *parent, const char *file_name, int32_t bytes_num, const uint8_t **bytes);

bool file_remove(inode_t *file);

#endif //FILE_SYSTEM_INODE_H
