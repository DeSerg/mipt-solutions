#include <stdio.h>

#include <server.h>

#include <file_system/mini_fs.h>
#include <string.h>
#include <tools/messages.h>

#define MAX_MES_LEN 300
#define MAX_ARG_NUM 30

bool parse_command(const char *full_command, char *command, char *args, int *arguments_num) {
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
    while (index_last < fc_len && full_command[index_last] != ' ') {
        ++index_last;
    }

    memcpy(command, full_command + index, index_last - index);
    command[index_last] = 0;

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

        memcpy(args + *arguments_num * MAX_MES_LEN, full_command + index, index_last - index);
        args[*arguments_num * MAX_MES_LEN + index_last - index] = 0;
        ++(*arguments_num);

        index = index_last;

        if (*arguments_num >= MAX_ARG_NUM) {
            fprintf(stderr, "main.c: parse_command: too much arguments: %d\n", *arguments_num);
            break;
        }

    }

    return true;
}

void obtain_commands() {

    char command[MAX_MES_LEN];
    char arguments[MAX_ARG_NUM][MAX_MES_LEN];
    arguments[0][0] = '1';
    arguments[0][1] = 0;

    int arguments_num = 0;

    ssize_t read = 0;
    size_t len = 0;
    char *line = NULL;

    while (1) {
        len = MAX_MES_LEN;
        ssize_t  bytes_read = getline(&line, &len, stdin);
        if (bytes_read == -1 || bytes_read == 0) {
            continue;
        }

        line[bytes_read - 1] = 0;
        parse_command(line, command, arguments[0], &arguments_num);

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

    free(line);

}



int FindUserCell()
{
    int i;
    for (i = 0; i < MAXUSERS; i++)
        if (users_available[i] == 0)
        {
            users_available[i] = 1;
            return i;
        }

    fprintf(stderr, "Error in usernumber\n");
    return -1;
}

int HandleMessage(MessageType* m, int incomeSd, int userID)
{
    if (m->type == login) //добавить пользователя и найти ему пару
    {
        // TODO
        return 0;
    }

    if (m->type == logout)
    {
        // TODO
        return 0;
    }

    if (m->type == command)
    {
        // TODO
        return 0;
    }

    return 0;
}

void* establishConnection(void* arg)
{
    //Мы работаем с конкретным соединением
    int incomeSd = *((int*)arg);
    int userID = FindUserCell();

    MessageType *buf = (MessageType*) malloc (sizeof(MessageType));
    int resultOfHandlingMessage = 1;

    while (resultOfHandlingMessage)
    {
        getMessage(incomeSd, buf);
        resultOfHandlingMessage = HandleMessage(buf, incomeSd, userID);
    }

    free(buf);
    return NULL;
}

void *manageConnections(void *arg)
{
    printf("Managing connectionsss...\n");
    fflush(stdout);

    int sd;
    pthread_t thread[MAXUSERS];

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);//псевдорандомно
    server.sin_addr.s_addr = INADDR_ANY; //слушаем все интрефейсы (wi-fi, провод и т.п.)

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1)
    {
        perror("Error while binding");
        exit(EXIT_FAILURE);
    }

    if (listen(sd, MAXUSERS) == -1)
    {
        perror("Error while listening");
        exit(EXIT_FAILURE);
    }

    int income_sd[MAXUSERS];

    int thread_num;
    for (thread_num = 0; thread_num < MAXUSERS; ++thread_num)
    {
        income_sd[thread_num] = accept(sd, 0, 0);

        printf("\n\nNew player connected\n");
        fflush(stdout);

        pthread_create(&(thread[thread_num]), 0, establishConnection, &(income_sd[thread_num]));
    }

    int i;
    for (i = 0; i <= thread_num; ++i)
    {
        pthread_join(thread[i], NULL);
        close(income_sd[i]);
    }

    close(sd);
    unlink(SOCKNAME);

    return NULL;
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
