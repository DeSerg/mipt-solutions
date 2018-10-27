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

bool get_free_inode(inode_t **inode);


// sector bit mask
bool read_sector_bit_mask();

bool update_sector_bit_mask(int ind, bool value);

bool clear_sector_bit_mask();

int32_t get_sector_address_by_index(int32_t sector_index);
bool get_free_sector_index(int32_t *sector_index);
bool get_free_sector_address(int32_t *sector_address);


// inode
bool read_inode(int index, inode_t *inode);

bool read_root_inode(inode_t *inode);

bool update_inode(int index, inode_t *inode);

bool remove_inode(int index);



#endif //FILE_SYSTEM_SUPERBLOCK_H
