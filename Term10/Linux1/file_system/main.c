#include <stdio.h>

#include <mini_fs.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Please pass file system file as 1st argument");
        return 1;
    }

    char *fileSystemFilepath = argv[1];

    FILE *fd = fopen(fileSystemFilepath, "wb+");
    ls(fd, "/");

    return 0;
}
