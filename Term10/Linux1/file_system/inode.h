//
// Created by serg on 30.09.18.
//

#ifndef FILE_SYSTEM_INODE_H
#define FILE_SYSTEM_INODE_H

#include <system.h>

typedef struct INode {


    int sectorPointers[INODE_SECTOR_POINTER_NUM];

} INode;



#endif //FILE_SYSTEM_INODE_H
