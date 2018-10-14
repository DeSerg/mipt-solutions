//
// Created by deserg on 14/10/2018.
//

#include <inode.h>

#include <files.h>
#include <superblock.h>
#include "system.h"

int32_t sectors_num_(int32_t bytes_num) {
    return (bytes_num - 1) / SECTOR_SIZE + 1;
}

bool read_inode_data(inode_t *directory, uint8_t **bytes) {

    *bytes = (int8_t *)calloc(directory->dataSize, sizeof(uint8_t));

    int32_t sectors_num = sectors_num_(directory->dataSize);

    for (int i = 0; i < sectors_num; ++i) {
        int32_t bytes_already_read = i * SECTOR_SIZE;
        int32_t bytes_left = directory->dataSize - bytes_already_read;
        int32_t bytes_to_read = (bytes_left / SECTOR_SIZE > 0) ? SECTOR_SIZE : bytes_left;

        if (!set_fd_to(directory->sectorPointers[i])) {
            fprintf(stderr, "inode.cpp: directory_inodes: failed to set fd to sector %d start", i);
            return false;
        }

        size_t bytes_read = read(Fd, *bytes + bytes_already_read, bytes_to_read);
        if (bytes_read != bytes_to_read) {
            fprintf(stderr, "inode.cpp: directory_inodes: "
                            "failed read %d sector: read %d bytes instead of %d", i, bytes_read, bytes_to_read);
            return false;
        }
    }

    return true;
}


bool allocate_data(int32_t bytes_num, uint8_t *bytes, int32_t *sectors_num, int32_t **sector_indeces) {

    *sectors_num = sectors_num_(bytes_left_to_write);
    *sector_indeces = (int32_t *)calloc(sectors_num, sizeof(int32_t));

    for (int i = 0; i < sectors_num; ++i) {
        if (!get_free_sector_index(sector_indeces + i)) {
            free(sector_indeces);
            fprintf(stderr, "inode.cpp: allocate_data: failed get free sector index");
            return false;
        }

        int32_t bytes_already_written = i * SECTOR_SIZE;
        int32_t bytes_left = bytes_num - bytes_already_written;
        int32_t bytes_to_write = (bytes_left / SECTOR_SIZE > 0) ? SECTOR_SIZE : bytes_left;

        int32_t sector_address = get_sector_address_by_index(sector_indeces[i]);
        if (!set_fd_to(sector_address)) {
            free(sector_indeces);
            fprintf(stderr, "inode.cpp: allocate_data: failed to move fd position to %d", sector_address);
            return false;
        }

        bytes_written = write(Fd, bytes, bytes_to_write);
        if (bytes_written != bytes_to_write) {
            free(sector_indeces);
            fprintf(stderr, "inode.cpp: allocate_data: failed to write data to %d sector: written "
                        "%d instead of %d bytes", sector_indeces[i], bytes_written, bytes_to_write);
            return false;
        }

        bytes += bytes_to_write;

    }

    return true;
}

bool append_inode_data(inode_t *inode, int32_t bytes_num, uint8_t *bytes) {

    if (!bytes_num) {
        printf("inode.cpp: append_inode_data: zero bytes array passed");
        return true;
    }

    // init essentials
    int32_t bytes_in_last_sector_busy = inode->dataSize % SECTOR_SIZE;
    int32_t bytes_in_last_sector_free = SECTOR_SIZE - bytes_in_last_sector_busy;

    int32_t last_sector_ind = (inode->dataSize - 1) / SECTOR_SIZE;

    int32_t bytes_left_to_write = 0;
    int32_t bytes_in_last_sector_to_write = 0;
    if (bytes_num > bytes_in_last_sector_free) {
        bytes_left_to_write = bytes_num - bytes_in_last_sector_free;
        bytes_in_last_sector_to_write = bytes_in_last_sector_free;
    } else {
        bytes_in_last_sector_to_write = bytes_num;
    }

    // allocate data in the last existing sector of inode
    if (!set_fd_to(inode->sectorPointers[last_sector_ind])) {
        fprintf(stderr, "inode.cpp: append_inode_data: failed to set fd to sector %d start", last_sector_ind);
        return false;
    }

    int32_t bytes_written = write(Fd, bytes, bytes_in_last_sector_to_write);
    if (bytes_written != bytes_in_last_sector_to_write) {
        fprintf(stderr, "inode.cpp: append_inode_data: failed to write data to %d sector: written "
                        "%d instead of %d bytes", last_sector_ind,  bytes_written, bytes_in_last_sector_to_write);
        return false;
    }

    if (bytes_left_to_write == 0) {
        inode->dataSize += bytes_num;
        return true;
    }

    // allocate the rest of the data in new sectors
    bytes += bytes_in_last_sector_to_write;
    int32_t sectors_num = 0;
    int32_t *sector_indeces = NULL;
    if (!allocate_data(bytes_left_to_write, bytes, &sectors_num, &sector_indeces)) {
        fprintf(stderr, "inode.cpp: append_inode_data: failed allocated data");
        return false;
    }

    // add new sectors to inode
    int32_t total_sectors_num = last_sector_ind + 1 + sectors_num;
    if (total_sectors_num >= INODE_SECTOR_POINTER_NUM) {
        free(sector_indeces);
        fprintf(stderr, "inode.cpp: append_inode_data: not enough pointers for sectors: "
                        "%d needed, %d actual", total_sectors_num, INODE_SECTOR_POINTER_NUM);
        return false;
    }

    for (int i = 0; i < sectors_num; ++i) {
        int sector_ind = last_sector_ind + 1 + i;
        inode->sectorPointers[sector_ind] = get_sector_address_by_index(sector_indeces[i]);
        if (!update_sector_bit_mask(sector_indeces[i], true)) {
            for (int j = 0; j < i; ++j) {
                update_sector_bit_mask(sector_indeces[j], false)
            }
            free(sector_indeces);
            fprintf(stderr, "inode.cpp: append_inode_data: failed to "
                            "update sector bit mask for index %d", sector_indeces[i]);
            return false;
        }
    }

    free(sector_indeces);
    inode->dataSize += bytes_num;
    return true;

}

bool directory_inodes(inode_t *directory, int32_t *inodes_count, inode_t **inodes, char ***filenames) {

    if (!directory->directory) {
        fprintf(stderr, "inode.cpp: directory_inodes: inode is not a directory!");
        return false;
    }

    uint8_t *bytes = NULL;
    if (!read_inode_data(directory, &bytes)) {
        free(bytes);
        fprintf(stderr, "inode.cpp: directory_inodes: failed to set read inode data");
        return false;
    }

    memcpy(inodes_count, (int32_t *)bytes, sizeof(int32_t));

    *inodes = (inode_t *)calloc(*inodes_count, sizeof(inode_t));
    *filenames = (char **)calloc(*inodes_count, sizeof(char *));

    uint8_t *bytes_current = bytes + 4;
    for (int i = 0; i < inode_number; ++i) {
        memcpy(*inodes + i, (int32_t *)bytes_current, sizeof(int32_t));
        bytes_current += 4;

        char *filename = (char *)bytes_current;
        int str_length = strlen(filename) + 1;
        bytes_current += str_length;

        (*filenames)[i] = calloc(str_length, sizeof(char));
        strcpy((*filenames)[i], filename);
    }

    free(bytes);
    return true;
}

bool directory_create(inode_t *parent, const char *directory_name) {

    inode_t *inode_free = NULL;
    if (!get_free_inode(&inode_free)) {
        fprintf(stderr, "inode.cpp: directory_create: failed to get free inode");
        return false;
    }

    int32_t directory_info_size = sizeof(int32_t) + (strlen(directory_name) + 1) * sizeof(char);
    uint8_t *directory_info = (uint8_t *)malloc(directory_info_size);
    memcpy(directory_info, &(inode_free->inode_index), sizeof(inode_free->inode_index));

    strcpy((char *)(directory_info + sizeof(inode_free->inode_index)), directory_name);

    bool success = append_inode_data(parent, directory_info_size, directory_info);
    if (!success) {
        fprintf(stderr, "inode.cpp: directory_create: failed to append new data to inode");
    }

    free(directory_info);
    return success;

}
