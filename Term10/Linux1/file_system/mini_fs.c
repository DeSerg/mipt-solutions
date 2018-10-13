#include <mini_fs.h>
#include <files.h>

bool check_fs(int fd) {
    int64_t fsize = file_size(fd);
    if (fsize < 0) {
        perror("mini_fs.c: check_fs: failed to check file size");
        return false;
    }

    return fsize == DISK_SIZE;
}

bool init_fs(int fd) {
    truncate_file(fd, DISK_SIZE);
    return true;
}

bool init_superblock() {

}

bool delete_superblock() {

}

bool prepare_fs(const char *filename) {

    Fd = open(filename, O_RDWR | O_CREAT);
    if (Fd == -1) {
        printf("mini_fs.c: prepare_fs: failed to open file %s", filename);
        return false;
    }

    if (!check_fs(Fd)) {
        printf("mini_fs.c: prepare_fs: fs is in invalid state, reiniting...");
        if (!init_fs(Fd)) {
            printf("mini_fs.c: prepare_fs: failed to open file %s", filename);
            return false;
        }
    }

    return true;
}

bool close_fs() {

    return close(Fd) == 0;

}

int ls(char *filepath) {

    return -1;
}


int cat(FILE *fd, char *filepath) {

    return -1;
}


int touch(FILE *fd, char *filepath, char *data) {

    return -1;
}


int copy(FILE *fd, char *filepath_source, char *filepath_dest) {

    return -1;
}


int move(FILE *fd, char *filepath_source, char *filepath_dest) {

    return -1;
}
