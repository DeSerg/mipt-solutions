#include <stdio.h>

#include <mini_fs.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Please pass file system file as 1st argument");
        return 1;
    }

    const char *mini_fs_filepath = argv[1];
    if (!prepare_fs(mini_fs_filepath)) {
        fprintf(stderr, "Failed to open mini_fs file %s", mini_fs_filepath);
        return -1;
    }

    ls("/");

    close_fs();
    return 0;
}
