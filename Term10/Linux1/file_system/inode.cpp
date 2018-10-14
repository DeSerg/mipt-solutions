//
// Created by deserg on 13/10/2018.
//
#include <inode.h>

bool read_inode_bit_mask(bool *inode_bit_mask) {

    if (lseek(Fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "inode.cpp: read_inode_bit_mask: failed to set fd to file begin");
        return false;
    }

    int ind = 0;
    for (int i = 0; i < inode_bit_mask_size(); ++i) {
        int bits_to_read = INODE_NUM - i * 8;
        uint32_t read_byte = 0;
        int bytes_read = pread(Fd, &read_byte, 1, inode_bit_mask_address() + i);
        if (bytes_read != 1) {
            fprintf(stderr, "inode.cpp: read_inode_bit_mask: failed to read inode: invalid bytes number: %d", bytes_read);
            return false;
        }

        uint32_t mask = 1 << 7;
        for (int shift = 0; shift < 8; ++shift, mask >>= 1) {
            inode_bit_mask[ind++] = mask & read_byte;
        }
    }

    return true;
}

bool update_inode_bit_mask(int ind, bool value) {
    if (lseek(Fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "inode.cpp: update_inode_bit_mask: failed to set fd to file begin");
        return false;
    }

    int byte_num = ind / 8;
    int shift = ind % 8;

    uint32_t mask = 0;
    uint32_t zero_mask = 0;
    for (int i = 0; i < 8; ++i) {
        if (i == shift) {
            mask |= value << i;
            zero_mask |= 0 << i;
        } else {
            mask |= 1 << i;
            zero_mask |= 1 << i;
        }
    }

    uint32_t read_byte = 0;
    if (pread(Fd, &read_byte, 1, inode_bit_mask_addres() + byte_num) == -1) {
        fprintf(stderr, "inode.cpp: update_inode_bit_mask: failed to read %d byte", inode_bit_mask_addres() + byte_num);
        return false;
    }

    read_byte &= zero_mask;
    read_byte |= mask;

    if (lseek(Fd, -1, SEEK_CUR) == -1) {
        fprintf(stderr, "inode.cpp: update_inode_bit_mask: failed to move fd cursor back");
        return false;
    }

    if (write(Fd, &read_byte, 1) == -1) {
        fprintf(stderr, "inode.cpp: update_inode_bit_mask: failed to write byte %d", read_byte);
        return false;
    }

    return true;
}


bool read_inode(int index, inode_t *inode) {

    if (lseek(Fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "inode.cpp: read_inode: failed to set fd to file begin");
        return false;
    }

    int bytes_read = pread(Fd, inode, INODE_SIZE, root_inode_address() + index * INODE_SIZE);
    if (bytes_read < INODE_SIZE) {
        fprintf(stderr, "inode.cpp: read_inode: failed to read inode: invalid bytes number: %d", bytes_read);
        return false;
    }

    return true;
}

bool update_inode(int index, inode_t *inode) {
    if (lseek(Fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "inode.cpp: update_inode: failed to set fd to file begin");
        return false;
    }

    int bytes_written = pwrite(Fd, inode, INODE_SIZE, root_inode_address() + index * INODE_SIZE);
    if (bytes_written < INODE_SIZE) {
        fprintf(stderr, "inode.cpp: update_inode: failed to write inode: invalid bytes number: %d", bytes_written);
        return false;
    }

    return true;
}

bool remove_inode(int index) {
    update_inode_bit_mask(index, false);
}
