#include <stdio.h>

#include <mini_fs.h>
#include <string.h>

#define MAX_MES_LEN 300
#define MAX_ARG_NUM 30

bool parse_command(const char *full_command, char *command, char **args, int *arguments_num) {
    int fc_len = (int)strlen(full_command);
    if (fc_len == 0) {
        return false;
    }

    int index = 0;
    while (index < fc_len && full_command[index] == ' ') {
        ++index;
    }

    if (index == fc_len) {
        return false;
    }

    int index_last = index;
    while (index_last < fc_len && index_last != ' ') {
        ++index_last;
    }

    memcpy(command, full_command + index, index_last - index);

    *arguments_num = 0;
    index = index_last;
    while (true) {
        while (index < fc_len && full_command[index] == ' ') {
            ++index;
        }

        if (index == fc_len) {
            break;
        }

        index_last = index;
        while (index_last < fc_len && index_last != ' ') {
            ++index_last;
        }

        memcpy(args[(*arguments_num)++], full_command + index, index_last - index);
        index = index_last;

        if (*arguments_num >= MAX_ARG_NUM) {
            fprintf(stderr, "main.c: parse_command: too much arguments: %d\n", *arguments_num);
            break;
        }

    }

    return true;
}

void obtain_commands() {

    char full_command[MAX_MES_LEN];
    char command[MAX_MES_LEN];
    char arguments[MAX_ARG_NUM][MAX_MES_LEN];
    int arguments_num = 0;

    while (1)
    {

        scanf("%s", full_command);
        parse_command(full_command, command, arguments, &arguments_num);

        if (!strcmp(command, "ls"))
        {
            if (arguments_num != 1) {
                printf("Usage: ls [file]\n");
                continue;
            }

            ls(arguments[0]);

        } else if (!strcmp(command, "mkdir")) {
            if (arguments_num != 1) {
                printf("Usage: mkdir [path]\n");
                continue;
            }

            mkdir(arguments[0]);
        }
        if (!strcmp(command, "exit"))
            break;
        else
        {
            printf("Invalid command, type \"help\" for help.\n");
        }
    }

}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Please pass file system file as 1st argument\n");
        return 1;
    }

    const char *mini_fs_filepath = argv[1];
    if (!prepare_fs(mini_fs_filepath)) {
        fprintf(stderr, "Failed to open mini_fs file %s\n", mini_fs_filepath);
        return 1;
    }

    obtain_commands();

    close_fs();
    return 0;
}
