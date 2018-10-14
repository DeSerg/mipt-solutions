#include <stdlib.h>

#include <mini_fs.h>
#include <files.h>

bool check_fs() {
    int64_t fsize = file_size(Fd);
    if (fsize < 0) {
        perror("mini_fs.c: check_fs: failed to check file size");
        return false;
    }

    return fsize == DISK_SIZE;
}

bool init_fs() {
    truncate_file(Fd, DISK_SIZE);
    return true;
}

// check contents of superblock
bool verify_superblock_() {
    if (!Superblock.inodes_used[0]) {
        fprintf(stderr, "mini_fs.c: verify_superblock: root inode is not used...");
        return false;
    }
    return true;
}

// reset contents of superblock
bool reset_superblock_() {

    if (!clear_inode_bit_mask()) {
        fprintf(stderr, "mini_fs.c: reset_superblock_: failed to clear inode bit mask");
        return false;
    }

    if (!clear_sector_bit_mask()) {
        fprintf(stderr, "mini_fs.c: reset_superblock_: failed to clear sector bit mask");
        return false;
    }

    if (!update_inode_bit_mask(0, true)) {
        fprintf(stderr, "mini_fs.c: reset_superblock_: failed to set root inode to used");
        return false;
    }

    return true;
}


// allocating bit maps and inodes, verifying data
bool init_superblock() {
    Superblock.inodes_used = (bool *)calloc((size_t)inode_bit_mask_size(), sizeof(bool));
    Superblock.inodes = (inode_t *)calloc((size_t)INODE_NUM, sizeof(inode_t));
    Superblock.sectors_used = (bool *)calloc((size_t)sector_bit_mask_size(), sizeof(bool));

    if (Superblock.inodes_used == NULL ||
        Superblock.inodes == NULL ||
        Superblock.sectors_used == NULL) {
        fprintf(stderr, "mini_fs.c: init_superblock: failed to allocate memory for superblock");
        return false;
    }

    if (!read_inode_bit_mask()) {
        fprintf(stderr, "mini_fs.c: init_superblock: failed to read inode bit mask");
        return false;
    }

    for (int i = 0; i < INODE_NUM; ++i) {
        if (!read_inode(i, Superblock.inodes + i)) {
            fprintf(stderr, "mini_fs.c: init_superblock: failed to read %d inode", i + 1);
            return false;
        }
    }

    if (!read_sector_bit_mask()) {
        fprintf(stderr, "mini_fs.c: init_superblock: failed to read sectors bit mask");
        return false;
    }

    if (!verify_superblock_()) {
        fprintf(stderr, "mini_fs.c: init_superblock: invalid superblock state, resetting...");
        if (!reset_superblock_()) {
            fprintf(stderr, "mini_fs.c: init_superblock: failed to reset superblock");
            return false;
        }
    }

    return true;
}

void delete_superblock() {
    free(Superblock.inodes_used);
    free(Superblock.inodes);
    free(Superblock.sectors_used);
}

bool prepare_fs(const char *filename) {

    Fd = open(filename, O_RDWR | O_CREAT);
    if (Fd == -1) {
        printf("mini_fs.c: prepare_fs: failed to open file %s", filename);
        return false;
    }

    if (!check_fs()) {
        printf("mini_fs.c: prepare_fs: fs is in invalid state, reiniting...");
        if (!init_fs()) {
            printf("mini_fs.c: prepare_fs: failed to open file %s", filename);
            return false;
        }
    }

    if (!init_superblock()) {
        printf("mini_fs.c: prepare_fs: failed to init superblock");
        delete_superblock();
        return false;
    }

    return true;
}

bool close_fs() {
    delete_superblock();
    return close(Fd) == 0;

}


int ls(char *filepath) {




    return -1;
}


int cat(char *filepath) {

    return -1;
}


int touch(char *filepath, char *data) {

    return -1;
}


int copy(char *filepath_source, char *filepath_dest) {

    return -1;
}


int move(char *filepath_source, char *filepath_dest) {

    return -1;
}
