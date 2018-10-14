#include <system.h>
#include <math.h>

const uint32_t SECTOR_SIZE = 1024; // in bytes

const uint32_t DISK_SIZE = 104857600; // (100 * 1024 * 1024 b or 100 Mb)


const int32_t ROOT_NODE_ADDRESS = 0;

const uint32_t INODE_NUM = 1024;



int32_t inode_bit_mask_address() {
    return ROOT_NODE_ADDRESS;
}

int32_t inode_bit_mask_size() {
    return (int32_t)ceil(INODE_NUM / 8.0);
}

int32_t root_inode_address() {
    return inode_bit_mask_address() + inode_bit_mask_size();
}

int32_t sector_bit_mask_address() {
    return root_inode_address() + INODE_NUM * sizeof(inode_t);
}


int32_t sector_bit_mask_size() {
    int32_t sectors_space = DISK_SIZE - sector_bit_mask_address();
    return (int32_t)ceil(sectors_space / (1.0 + 8 * SECTOR_SIZE));
}

int32_t first_sector_address() {
    return sector_bit_mask_address() + sector_bit_mask_size();
}

int32_t sectors_number() {
    return (DISK_SIZE - first_sector_address()) / SECTOR_SIZE;
}
