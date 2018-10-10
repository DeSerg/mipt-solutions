//
// Created by serg on 30.09.18.
//

#ifndef FILE_SYSTEM_SYSTEM_H
#define FILE_SYSTEM_SYSTEM_H

#include <stdint.h>


extern const uint32_t SECTOR_SIZE; // in bytes

extern const uint64_t DISK_SIZE;  // in bytes


#define INODE_SECTOR_POINTER_NUM 1024

typedef struct INode {

    int32_t dataSize;
    int32_t pointersNum;
    int32_t sectorPointers[INODE_SECTOR_POINTER_NUM];

} INode;


extern const int32_t ROOT_NODE_ADDRESS;

extern const uint32_t INODE_NUM;

#define FIRST_SECTOR_ADDRESS (ROOT_NODE_ADDRESS + INODE_NUM * sizeof(INode))



#endif //FILE_SYSTEM_SYSTEM_H
