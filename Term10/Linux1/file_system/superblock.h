//
// Created by deserg on 30.09.18.
//

#ifndef FILE_SYSTEM_SUPERBLOCK_H
#define FILE_SYSTEM_SUPERBLOCK_H

#include <system.h>

// inode bit mask
bool read_inode_bit_mask();

bool update_inode_bit_mask(int ind, bool value);

bool clear_inode_bit_mask();

// sector bit mask
bool read_sector_bit_mask();

bool update_sector_bit_mask(int ind, bool value);

bool clear_sector_bit_mask();

// inode
bool read_inode(int index, inode_t *inode);

bool update_inode(int index, inode_t *inode);

bool remove_inode(int index);



#endif //FILE_SYSTEM_SUPERBLOCK_H
