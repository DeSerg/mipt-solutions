//
// Created by deserg on 30.09.18.
//

#ifndef FILE_SYSTEM_INODE_H
#define FILE_SYSTEM_INODE_H

#include <system.h>


bool read_inode(int fd, int index, inode_t *inode);

bool update_inode(int fd, int index, inode_t *inode);

bool remove_inode(int fd, int index);

#endif //FILE_SYSTEM_INODE_H
