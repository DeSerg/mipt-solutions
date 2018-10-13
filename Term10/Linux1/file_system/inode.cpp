//
// Created by deserg on 13/10/2018.
//
#include <inode.h>

bool read_inode(int fd, int index, INode *inode) {

    int bytes_read = pread(fd, inode, INODE_SIZE, FIRST_INODE_ADDRESS + index * INODE_SIZE);
    if (bytes_read < INODE_SIZE) {
        fprintf(stderr, "inode.cpp: read_inode: failed to read inode: invalid bytes number: %d", bytes_read);
        return false;
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "inode.cpp: read_inode: failed to set fd to file begin");
        return false;
    }

    return true;
}

bool update_inode(int fd, int index, INode *inode) {

}

bool remove_inode(int fd, int index) {

}
