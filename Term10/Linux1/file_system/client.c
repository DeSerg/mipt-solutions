//
// Created by deserg on 18/11/2018.
//
#include <stdio.h>
#include <tools/messages.h>


UserData user_data;

int sd;
int login_s;
pthread_mutex_t print_mutex;


void ClearScreen()
{
    system("clear");
}


void ScanIP(char *address)
{
    ClearScreen();
    pthread_mutex_lock(&print_mutex);
    printf("Please, insert the address\n");
    pthread_mutex_unlock(&print_mutex);
    scanf("%s", address);
}


void GetIP(char *address)
{
    ClearScreen();
    printf("Please, choose the option:\n");
    printf("\t't' - play chess on this PC\n");
    printf("\t'i' - connect to another PC using IP\n");

    char option[MAX_MES_LEN];

    pthread_mutex_lock(&print_mutex);
    scanf("%s", option);
    pthread_mutex_unlock(&print_mutex);

    if (strcmp(option, "t") && strcmp(option, "i"))
        while (strcmp(option, "t") && strcmp(option, "i"))
        {
            ClearScreen();
            printf("Wrong symbol\n");
            printf("Please, choose the option:\n");
            printf("\t't' - play chess on this PC\n");
            printf("\t'i' - connect to another PC using IP\n");

            pthread_mutex_lock(&print_mutex);
            scanf("%s", option);
            pthread_mutex_unlock(&print_mutex);
        }

    if (!strcmp(option, "t"))
        strcpy(address, "127.0.0.1");
    else
        ScanIP(address);

}


void UserLogin()
{
    ClearScreen();

    //reading user info
    printf("You were successfully connected to server!\n\n");
    printf("Please, enter your name:\n");
    pthread_mutex_lock(&print_mutex);
    scanf("%s", user_data.name);
    pthread_mutex_unlock(&print_mutex);

    printf("\n\nGood job. Now you will be connected to the server\n");

    //trying to log user in
    MessageType m = composeMessage(login, sizeof(UserData), &user_data);
    sendMessage(sd, &m);
    getMessage(sd, &m);

    if (m.type != result)
    {
        ClearScreen();
        perror("Wrong message!");
        exit(1);
    }
    else
    {
        int result_val = *((int *) m.data);
        if (result_val == 1)
        {
            ClearScreen();
            printf("Welcome, %s!\n", user_data.name);

            getMessage(sd, &m);
            int result_val = *((int *) m.data);
            if (!result_val)
            {
                printf("Wait a little, your game will start soon...\n");
            }

            getMessage(sd, &m);
            char str[MAXNAMELENGTH];
            strcpy(str, (char *) m.data);
            getMessage(sd, &m);

            printf("\n\nInsert 'help' to see some useful information\n");
        }
        else
        {
            ClearScreen();
            printf("Sorry, there are no free slots for using remote file system...\n");
            printf("Goodbye :*\n");
            exit(0);
        }
    }

    login_s = 1;
    clearMessage(&m);
}


void UserLogout(int screen)
{
    if (sd == -1)
    {
        if (screen)
            ClearScreen();
        printf("Goodbye, have a nice day:)\n");
        exit(0);
    }

    MessageType m = composeMessage(logout, 0, NULL);
    sendMessage(sd, &m);
    getMessage(sd, &m);
    if (m.type != result)
    {
        if (screen)
            ClearScreen();
        perror("Wrong message type");
        exit(1);
    }
    else
    {
        char result_val = * (char *) m.data;
        if (result_val == 'g')
        {
            if (screen)
                ClearScreen();
            printf("Goodbye, have a nice day:)\n");
            exit(0);
        }
        if (!result_val)
        {
            if (screen)
                ClearScreen();
            perror("Logout was not comlpete\n");
            exit(0);
        }
    }
    clearMessage(&m);

}


void *ManageCommands(void *arg)
{
    ssize_t read = 0;
    size_t len = 0;
    char *line = NULL;

    while (1)
    {
        len = MAX_MES_LEN;
        ssize_t  bytes_read = getline(&line, &len, stdin);
        if (bytes_read == -1 || bytes_read == 0) {
            continue;
        }

        if (!strcmp(line, "quit"))
            UserLogout(1);

    }

    return NULL;
}


int main()
{
    printf("Hello! Welcome to Serg's file system!\n");

    sd = -1;
    login_s = 0;
    struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

    char address[MAX_IP_LENGTH];
    GetIP(address);

    struct hostent *srv_host;
    srv_host = gethostbyname(address);
    memcpy((char *)&server.sin_addr.s_addr, (char *)srv_host->h_addr_list[0], srv_host->h_length);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        ClearScreen();
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    if (connect(sd, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == -1)
    {
        ClearScreen();
        perror("Error while connecting to socket");
        exit(EXIT_FAILURE);
    }

    UserLogin();

    pthread_t command_thread;
    pthread_create(&command_thread, 0, ManageCommands, 0);
    pthread_join(command_thread, 0);

    return 0;
}