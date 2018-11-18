//
// Created by deserg on 13/10/2018.
//

#include <system.h>
#include <file_system/files.h>
#include <file_system/superblock.h>


// common bit mask methods
int32_t bytes_num_(int32_t bits_num) {
    return (bits_num - 1) / 8 + 1;
}

bool read_bit_mask_(bool *bit_mask, int32_t bits_count, int32_t fd_offset) {
    if (!set_fd_to_start()) {
        fprintf(stderr, "superblock.cpp: read_bit_mask_: failed to set fd to file begin\n");
        return false;
    }

    int ind = 0;
    int32_t bytes_count = bytes_num_(bits_count);
    for (int i = 0; i < bytes_count; ++i) {
        int bits_left = bits_count - i * 8;
        int bits_to_read = (bits_left / 8 > 0) ? 8 : bits_left;

        uint32_t read_byte = 0;
        int bytes_read = pread(Fd, &read_byte, 1, fd_offset + i);
        if (bytes_read != 1) {
            fprintf(stderr, "superblock.cpp: read_bit_mask_: failed to read byte: invalid bytes number: %d\n", bytes_read);
            return false;
        }

        uint32_t mask = 1 << 7;
        for (int shift = 0; shift < bits_to_read; ++shift, mask >>= 1) {
            bit_mask[ind++] = mask & read_byte;
        }
    }

    return true;
}

bool update_bit_mask_(bool *bit_mask, int ind, bool value, int32_t fd_offset) {
    if (!set_fd_to_start()) {
        fprintf(stderr, "superblock.cpp: update_bit_mask_: failed to set fd to file begin\n");
        return false;
    }

    int byte_num = ind / 8;
    int shift = 7 - ind % 8;

    uint8_t mask = value << shift;
    uint8_t zero_mask = ~(1 << shift);

    uint8_t read_byte = 0;
    if (pread(Fd, &read_byte, 1, inode_bit_mask_address() + byte_num) == -1) {
        fprintf(stderr, "superblock.cpp: update_bit_mask_: failed to read %d byte\n", fd_offset + byte_num);
        return false;
    }

    read_byte &= zero_mask;
    read_byte |= mask;

    ssize_t written = write(Fd, &read_byte, 1);
    if (written != 1) {
        fprintf(stderr, "superblock.cpp: update_bit_mask_: failed to write byte %d\n", read_byte);
        perror("");
        return false;
    }

    bit_mask[ind] = value;

    return true;
}

bool clear_bit_mask_(bool *bit_mask, int32_t bits_count, int32_t fd_offset) {
    if (lseek(Fd, fd_offset, SEEK_SET) == -1) {
        fprintf(stderr, "superblock.cpp: clear_bit_mask_: failed to set fd to file begin\n");
        return false;
    }

    int32_t bytes_count = bytes_num_(bits_count);
    int zero_byte = 0;
    int bytes_written = 0;
    for (int i = 0; i < bytes_count; ++i) {
        bytes_written = write(Fd, &zero_byte, 1);
        if (bytes_written != 1) {
            fprintf(stderr, "superblock.cpp: clear_bit_mask_: failed to write %d zero byte\n", i + 1);
        }
    }

    memset(bit_mask, zero_byte, bits_count * sizeof(bool));
    return true;
}

// inode bit mask
bool read_inode_bit_mask() {
    return read_bit_mask_(Superblock.inodes_used, inode_bit_mask_size(), inode_bit_mask_address());
}

bool update_inode_bit_mask(int ind, bool value) {
    return update_bit_mask_(Superblock.inodes_used, ind, value, inode_bit_mask_address());
}

bool clear_inode_bit_mask() {
    return clear_bit_mask_(Superblock.inodes_used, inode_bit_mask_size(), inode_bit_mask_address());
}

bool get_free_inode(inode_t **inode) {

    for (int i = 0; i < inode_bit_mask_size(); ++i) {
        if (!Superblock.inodes_used[i]) {
            *inode = Superblock.inodes +i;
            return true;
        }
    }

    fprintf(stderr, "superblock.cpp: get_free_inode: all free inodes are gone... Sorry...\n");
    return false;
}


// sector bit mask
bool read_sector_bit_mask() {
    return read_bit_mask_(Superblock.sectors_used, sector_bit_mask_size(), sector_bit_mask_address());
}

bool update_sector_bit_mask(int ind, bool value) {
    return update_bit_mask_(Superblock.sectors_used, ind, value, sector_bit_mask_address());
}

bool clear_sector_bit_mask() {
    return clear_bit_mask_(Superblock.sectors_used, sector_bit_mask_size(), sector_bit_mask_address());
}

int32_t get_sector_address_by_index(int32_t sector_index) {
    return first_sector_address() + sector_index * SECTOR_SIZE;
}

bool get_free_sector_index(int32_t *sector_index) {
    for (int i = 0; i < sector_bit_mask_size(); ++i) {
        if (!Superblock.sectors_used[i]) {
            *sector_index = i;
            return true;
        }
    }

    fprintf(stderr, "superblock.cpp: get_free_sector_index: all free sectors are gone... Sorry...\n");
    return false;
}

bool get_free_sector_address(int32_t *sector_address) {
    int32_t free_sector_index = 0;
    if (!get_free_sector_index(&free_sector_index)) {
        return false;
    }

    *sector_address = get_sector_address_by_index(free_sector_index);
    return true;
}


// inode
bool read_inode(int index, inode_t *inode) {
    if (!set_fd_to_start()) {
        fprintf(stderr, "superblock.cpp: read_inode: failed to set fd to file begin\n");
        return false;
    }

    if (index >= INODE_NUM) {
        fprintf(stderr, "superblock.cpp: read_inode: index %d is "
                        "greater then max number of inodes %d\n", index, INODE_NUM);
        return false;
    }

    if (!Superblock.inodes_used[index]) {
        fprintf(stderr, "superblock.cpp: read_inode: inode with index %d is not busy\n", index);
        return false;
    }

    int bytes_read = pread(Fd, inode, INODE_SIZE, root_inode_address() + index * INODE_SIZE);
    if (bytes_read < INODE_SIZE) {
        fprintf(stderr, "superblock.cpp: read_inode: failed to read inode: invalid bytes number: %d\n", bytes_read);
        return false;
    }

    return true;
}

bool read_root_inode(inode_t *inode) {
    return read_inode(0, inode);
}

bool update_inode(int index, inode_t *inode) {
    if (!set_fd_to_start()) {
        fprintf(stderr, "superblock.cpp: update_inode: failed to set fd to file begin\n");
        return false;
    }

    ssize_t bytes_written = pwrite(Fd, inode, INODE_SIZE, root_inode_address() + index * INODE_SIZE);
    if (bytes_written < INODE_SIZE) {
        fprintf(stderr, "superblock.cpp: update_inode: failed to write inode: invalid bytes number: %zi\n", bytes_written);
        return false;
    }

    return true;
}

bool remove_inode(int index) {
    return update_inode_bit_mask(index, false);
}
