//
// Created by deserg on 30.09.18.
//

#ifndef FILE_SYSTEM_SYSTEM_H
#define FILE_SYSTEM_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

#define INODE_SECTOR_POINTER_NUM 1024

typedef struct inode_t {

    int32_t dataSize;
    int32_t sectorPointers[INODE_SECTOR_POINTER_NUM];

} inode_t;

typedef struct superblock_t {

    int8_t *inodes_used;
    inode_t *inodes;

    int8_t *sectors_used;

} superblock_t;

extern const uint32_t SECTOR_SIZE; // in bytes

extern const uint32_t DISK_SIZE;  // in bytes

extern const int32_t ROOT_NODE_ADDRESS;

extern const uint32_t INODE_NUM;

#define INODE_SIZE sizeof(inode_t)

int32_t sectors_number();

int32_t inode_bit_mask_address();
int32_t inode_bit_mask_size();

int32_t root_inode_address();

int32_t sector_bit_mask_address();
int32_t sector_bit_mask_size();

int32_t first_sector_address();

#define FIRST_SECTOR_ADDRESS (ROOT_NODE_ADDRESS + INODE_NUM * INODE_SIZE)


// Global variables
// superblock
superblock_t Superblock;

// minifs file descriptor
int Fd;

#endif //FILE_SYSTEM_SYSTEM_H
