#include "server.h"


void ClearScreen()
{
    system("clear");
}


int CanPlay()
{
    if (numberOfPlayers == MAXUSERS)
    {
        return 0;
    }
    else 
    {
        pthread_mutex_lock(&usnum_mutex);
        numberOfPlayers++;
        pthread_mutex_unlock(&usnum_mutex);
        return 1;
    }
}


int FindCell()
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


void SetDisposition(int gameID)
{
    int i;
    for (i = 0; i < 64; i++)
        games[gameID].d[i].type = none;
    
    for (i = 0; i < 16; i++)
        games[gameID].d[i].color = white;
    for (i = 63; i > 47; i--)
        games[gameID].d[i].color = black;
    
    games[gameID].d[0].type = rook;
    games[gameID].d[7].type = rook;
    games[gameID].d[56].type = rook;
    games[gameID].d[63].type = rook;
    
    games[gameID].d[1].type = knight;
    games[gameID].d[6].type = knight;
    games[gameID].d[57].type = knight;
    games[gameID].d[62].type = knight;
    
    games[gameID].d[2].type = bishop;
    games[gameID].d[5].type = bishop;
    games[gameID].d[58].type = bishop;
    games[gameID].d[61].type = bishop;
    
    games[gameID].d[4].type = king;
    games[gameID].d[60].type = king;
    
    games[gameID].d[3].type = queen;
    games[gameID].d[59].type = queen;
    
    for (i = 8; i <= 15; i++)
        games[gameID].d[i].type = pawn;
    
    for (i = 48; i <= 55; i++)
        games[gameID].d[i].type = pawn;    
}


void PrintUserByID(int userID)
{
    printf("\n\nUser data is:\n");
    printf("\tName: %s\n", users[userID].name);
    printf("\tOwn and desired level: %d, %d\n", users[userID].ownLevel, users[userID].desiredLevel);
    printf("\tUserID: %d\n", users[userID].userID);
    
}


void PrintUser(UserData *user_data)
{
    printf("\n\nUser data is:\n");
    printf("\tName: %s\n", user_data->name);
    printf("\tOwn and desired level: %d, %d\n", user_data->ownLevel, user_data->desiredLevel);
    printf("\tUserID: %d\n", user_data->userID);
    
}


int CreateGame(int user1, int user2)
{
    int i;
    PrintUserByID(user1);
    PrintUserByID(user2);
    for (i = 0; i < MAXGAMES; i++)
        if (games_available[i] == 0)
        {
            SetDisposition(i);
            int choice = rand() % 2;
            if (choice)
            {
                memcpy(&games[i].white_user, &users[user1], sizeof(UserData));
                memcpy(&games[i].black_user, &users[user2], sizeof(UserData));
            }
            else
            {
                memcpy(&games[i].white_user, &users[user2], sizeof(UserData));
                memcpy(&games[i].black_user, &users[user1], sizeof(UserData));
            }
            
            games[i].current = white;
            return i;
        }
    
    perror("Wrong game number");
    return 0;
}


void FindOpponent(int incomeSd, int userID)
{
    int found = 0;
    int i;
    for (i = 0; i < MAXUSERS; i++)
        if (i != userID && users_available[i])
        {
            if (users[userID].ownLevel == users[i].desiredLevel && users[userID].desiredLevel == users[i].ownLevel)
            {
                found = 1;
                int gameID = CreateGame(userID, i);
                userGame[userID] = userGame[i] = gameID;
                userOp[userID] = i;
                userOp[i] = userID;
                
                printf("\n\nUserIDs: %d, %d\n", userID, i);
                printf("Game info\n");
                printf("\tGameID: %d\n", gameID);
                printf("\tWhiteID: %d\n", games[gameID].white_user.userID);
                printf("\tBlackID: %d\n", games[gameID].black_user.userID);
                break;
            }
        }
    
    if (!found)
    {
        int result_val = 0;
        MessageType m = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &m);
        
        while (userOp[userID] == userID)
            continue;
    }
    else
    {
        int result_val = 1;
        MessageType m = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &m);
        
    }
    
    MessageType m = composeMessage(text, sizeof(users[userOp[userID]].name), &(users[userOp[userID]].name));
    sendMessage(incomeSd, &m);
    
    Color color;
    if (games[userGame[userID]].white_user.userID == userID)
        color = white;
    else
        color = black;
    m = composeMessage(color_t, sizeof(Color), &color);
    sendMessage(incomeSd, &m);
    
    clearMessage(&m);    
}


int UserLogin(MessageType* m, int incomeSd, int userID)
{
    printf("\n\nNew user [%d] is logging in!\n", userID);
    fflush(stdout);
    
    UserData user_data = *(UserData*)(m->data);
    users[userID] = user_data; //ud никуда не денется, потому что в куче
    users[userID].userID = userID;
    
    PrintUserByID(userID);    
    
    if (!CanPlay())
    {
        printf("User %s will not be logged in\n", user_data.name);
        fflush(stdout);

        int result_val = 0;
        MessageType m = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &m);
        clearMessage(&m);
        
        return 0;
    }
    else
    {
        printf("User %s has successfully logged in!\n", user_data.name);
        fflush(stdout);
        
        int result_val = 1;
        MessageType m = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &m);

        FindOpponent(incomeSd, userID);
        
        clearMessage(&m);
        
        return 1;
    }    
    
}


int UserLogout(int incomeSd, int userID)
{
    printf("\n\nUser [%d] is logging out!\n", userID);
    fflush(stdout);
    
    users_available[userID] = 0;
    pthread_mutex_lock(&usnum_mutex);
    numberOfPlayers--;
    pthread_mutex_unlock(&usnum_mutex);    
    
    if (userGame[userID] >= 0)
    {
        games_available[userGame[userID]] = 0;
        userGame[userID] = -1;
        userGame[userOp[userID]] = -1;
        userOp[userOp[userID]] = userOp[userID];
        userOp[userID] = userID;
    }
    
    printf("Userdata cleared...\n");
    fflush(stdout);
    
    char result_val = 'g';
   
    MessageType m = composeMessage(result, sizeof(char), &result_val);
    sendMessage(incomeSd, &m);
    clearMessage(&m);
    
    printf("User %s was successfully logged out\n", users[userID].name);
    fflush(stdout);
    
    return 0;
}


int UserDisposition(int incomeSd, int userID)
{
    printf("\n\nUser %s is getting a disposition!\n", users[userID].name);
    
    MessageType m;
    int result_val;
    if (userGame[userID] == -1)
        result_val = 0;
    else
        result_val = 1;
    
    m = composeMessage(result, sizeof(int), &result_val);
    sendMessage(incomeSd, &m);
    
    if (result_val)
    {
        m = composeMessage(disposition, sizeof(games[userGame[userID]].d), &(games[userGame[userID]].d));
        sendMessage(incomeSd, &m);
        
        printf("Disposition was sent\n");
        return 1;
    }
    else
        return 0;
}


void MakeTurn(char *turn_str, int gameID)
{
    int n1 = turn_str[0] - 'a';
    int n2 = turn_str[1] - '1';
    int n3 = turn_str[2] - 'a';
    int n4 = turn_str[3] - '1';
    
    int pos1 = 8 * n2 + n1;
    int pos2 = 8 * n4 + n3;
    
    memcpy(&games[gameID].d[pos2], &games[gameID].d[pos1], sizeof(Disposition));
    games[gameID].d[pos1].type = none;
        
}

int IsEmpty(int n1, int n2, Disposition *d)
{
    if (d[n2 * 8 + n1].type == none)
        return 1;
    else
        return 0;
}

int CheckBetween(int n1, int n2, int n3, int n4, Disposition *d)
{
    int step1, step2;
    if (n1 > n3) 
        step1 = -1;
    else
    if (n1 < n3)
        step1 = 1;
    else
        step1 = 0;
    
    if (n2 > n4) 
        step2 = -1;
    else
    if (n2 < n4)
        step2 = 1;
    else
        step2 = 0;
    
    int i, j;
    int result = 1;
    for (i = n1 + step1, j = n2 + step2; i != n3 || j != n4; i += step1, j += step2)
        if (!IsEmpty(i, j, d))
        {
            result = 0;
            break;
        }
    
    return result;
}


int CheckMove(int n1, int n2, int n3, int n4, char *error, Game *game)
{
    Color color = game->current;
    Disposition d1 = game->d[n2 * 8 + n1];
    Disposition d2 = game->d[n4 * 8 + n3];
    
    
    
    if (d1.type != none && d1.color != color)
    {
        strcpy(error, "You are trying to use enemy's figure!");
        return 0;
    }
    if (d2.type != none && d2.color == color)
    {
        strcpy(error, "You are trying to eat your figure!");
        return 0;
    }
    
    switch (d1.type)
    {
    case pawn:
        if (n1 == n3)
        {
            //good moving side
            if ((color == white && (n4 - n2 < 1 || n4 - n2 > 2))  ||  (color == black && (n4 - n2 < -2 || n4 - n2 > -1)))
            {
                strcpy(error, "Your pawn has an incorrect destination cell");
                return 0;
            }
            if (!CheckBetween(n1, n2, n3, n4, game->d))
            {
                strcpy(error, "There is a figure between start and finish of your pawn's way");
                return 0;
            }
        }
        else
        if (abs(n1 - n3) == 1)
        {
            if ((color == white && (n4 - n2 < 2 || n4 - n2 > 3))  ||  (color == black && (n4 - n2 < -3 || n4 - n2 > -2)))
            {
                strcpy(error, "Your pawn has an incorrect destination cell");
                return 0;
            }
            
            if (color == white && n4 - n2 == 2 && !IsEmpty(n1, n2 + 1, game->d))
            {
                strcpy(error, "There is a figure between start and finish of your pawn's way");
                return 0;
            }
                
            
            if (color == black && n4 - n2 == -2 && !IsEmpty(n1, n2 - 1, game->d))
            {
                strcpy(error, "There is a figure between start and finish of your pawn's way");
                return 0;
            }
                
            
            if (color == white && (d2.type == none || d2.color == white))
            {
                strcpy(error, "You can't make passant to an empty cell");
                return 0;
            }
            
            if (color == black && (d2.type == none || d2.color == black))
            {
                strcpy(error, "You can't make passant to an empty cell");
                return 0;
            }
        }
        else
        {
            strcpy(error, "Your pawn has an incorrect destination cell");
            return 0;
        }
                    
        break;
    
    case knight:
        if (!(((abs(n3 - n1) == 2) && (abs(n4 - n2) == 1)) || ((abs(n3 - n1) == 1) && (abs(n4 - n2) == 2))))
        {
            strcpy(error, "Your knight has an incorrect destination cell");
            return 0;
        }        
        
        break;
        
    case bishop:
        if (abs(n3 - n1) != abs(n4 - n2))
        {
            strcpy(error, "Your bishop has an incorrect destination cell");
            return 0;
        }
        
        if (!CheckBetween(n1, n2, n3, n3, game->d))
        {
            strcpy(error, "There is a figure between start and finish of your bishop's way");
            return 0;
        }            
        
        break;
        
    case rook:
        
        if (n1 != n3 && n2 != n4)
        {
            strcpy(error, "Your rook has an incorrect destination cell");
            return 0;
        }
        
        if (!CheckBetween(n1, n2, n3, n3, game->d))
        {
            strcpy(error, "There is a figure between start and finish of your rook's way");
            return 0;
        }                    
        
        break;
        
    case queen:
        if (!((abs(n3 - n1) == abs(n4 - n2)) || (n1 == n3) || (n2 == n4)))
        {
            strcpy(error, "Your queen has an incorrect destination cell");
            return 0;
        }

        if (!CheckBetween(n1, n2, n3, n3, game->d))
        {
            strcpy(error, "There is a figure between start and finish of your queen's way");
            return 0;
        }                    
        
        break;
        
    case king:
        if (abs(n3 - n1) > 1 || abs(n4 - n2) > 1)
        {
            strcpy(error, "Your king has an incorrect destination cell");
            return 0;
        }

        break;
        
    case none:
        strcpy(error, "You are trying to make a turn without a figure");
        return 0;
        break;
    }
    
    return 1;
}


int HandleTurn(char *turn_str, char *error, Game *game)
{
    printf("Turn now will be checked\n");
    if (strlen(turn_str) != 4)
    {
        printf("Turn has invalid length (%d)\n", (int)strlen(turn_str));
        strcpy(error, "Turn has invalid length");
        return 0;
    }
    
    char c1 = turn_str[0];
    char c2 = turn_str[1];
    char c3 = turn_str[2];
    char c4 = turn_str[3];
    
    printf("%c %c %c %c\n", c1, c2, c3, c4);
    
    if (!(c1 >= 'a' && c1 <= 'h' && c2 >= '1' && c2 <= '8' && c3 >= 'a' && c3 <= 'h' && c4 >= '1' && c4 <= '8'))
    {
        printf("Turn consists from invalid symbols\n");
        strcpy(error, "Turn consists from invalid symbols\n");
        return 0;
    }
    
    if (!CheckMove(c1 - 'a', c2 - '1', c3 - 'a', c4 - '1', error, game))
    {
        printf("%s\n", error);
        return 0;
    }
    
    printf("The turn is correct!\n");
    return 1;
}



int UserTurn(MessageType *m, int incomeSd, int userID)
{
    printf("\n\nHandling the turn\n");
    fflush(stdout);
        
    int result_val;
    int gameID = userGame[userID];
    
    char turn_str[MAX_MES_LEN];
    char error[MAX_MES_LEN];
    strcpy(turn_str, (char *)m->data);
    
    if(userGame[userID] == -1)
    {
        result_val = 2;
        printf("The game of %s is over, he will be disconnected", users[userID].name);
        fflush(stdout);
        
        MessageType mes = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &mes);
        clearMessage(&mes);
    }
    else
    if (HandleTurn(turn_str, error, &games[gameID]))
    {
        result_val = 1;
        printf("Making turn\n");

        MakeTurn(turn_str, gameID);        
        
        printf("Color before: %d\n", games[gameID].current);
        games[gameID].current = (games[gameID].current + 1) % 2;        
        printf("Color after: %d\n", games[gameID].current);
        fflush(stdout);
        
        MessageType mes = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &mes);
        clearMessage(&mes);
        
    }
    else
    {
        result_val = 0;
        MessageType mes = composeMessage(result, sizeof(int), &result_val);
        sendMessage(incomeSd, &mes);
        
        mes = composeMessage(text, sizeof(error), error);
        sendMessage(incomeSd, &mes);        
        clearMessage(&mes);
    }

    
    printf("Handling turn is finished\n");
    fflush(stdout);
    
    return 1;
}


int UserTryTurn(int incomeSd, int userID)
{
    printf("\n\nUser %s is trying to make a turn!\n", users[userID].name);
    fflush(stdout);
    
    int result_val;
    if (userGame[userID] == -1)
    {    
        printf("The game of %s is over, he will be disconnected", users[userID].name);
        fflush(stdout);
        result_val = 0;
    }
    else
    {
        int gameID = userGame[userID];
        Game game = games[gameID];
        
        printf("The id of the game is: %d\n", gameID);

        printf("UserID: %d\n", userID);
        printf("WhiteID: %d\n", game.white_user.userID);
        printf("BlackID: %d\n", game.black_user.userID);

        
        if (game.current == white)
            printf("Current turn is whites'\n");
        else
            printf("Current turn is blacks'\n");
        if ((game.current == white && game.white_user.userID != userID) || (game.current == black && game.black_user.userID != userID))
        {
            printf("Not time for turn!\n");
            result_val = 1;
        }
        else
            result_val = 2;
    }
    
    MessageType mes = composeMessage(result, sizeof(int), &result_val);
    sendMessage(incomeSd, &mes);
    
    if (result_val == 2)
    {
        getMessage(incomeSd, &mes);
        UserTurn(&mes, incomeSd, userID);
    }
    clearMessage(&mes);
    
    return 1;
}


int HandleMessage(MessageType* m, int incomeSd, int userID) 
{
    if (m->type == login) //добавить пользователя и найти ему пару 
    {
        return UserLogin(m, incomeSd, userID);
    }
    else
    if (m->type == logout)
    {   
        return UserLogout(incomeSd, userID);
    }
    if (m->type == disposition)
    {
        return UserDisposition(incomeSd, userID);
    }    
    if (m->type == try_turn)
    {
        return UserTryTurn(incomeSd, userID);
    }
    if (m->type == turn)
    {
        return UserTurn(m, incomeSd, userID);
    }
    return 0;
}



void* establishConnection(void* arg) 
{
    //Мы работаем с конкретным соединением
    int incomeSd = *((int*)arg);
            
    int userID = FindCell();

    MessageType *buf = (MessageType*) malloc (sizeof(MessageType));
    int resultOfHandlingMessage = 1;

    while (resultOfHandlingMessage) 
    {
        if (server_enabled)
        {
            getMessage(incomeSd, buf);
            resultOfHandlingMessage = HandleMessage(buf, incomeSd, userID);
        }
        else
        {
            char *str = "Server is turned off...";
            MessageType m = composeMessage(text, strlen(str), str);
            sendMessage(incomeSd, &m);
        }
    }   
    
    free(buf);
    return NULL;
}


void handler(int sigVal)
{
    if (sigVal == SIGINT)
    {
        ClearScreen();
         printf("Server has been disabled...\n");
        exit(0);
    }
}


void ObtainSignals()
{
    signal(SIGINT, &handler);
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
        ClearScreen();
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1)
    {
        ClearScreen();
        perror("Error while binding");
        exit(EXIT_FAILURE);
    }
    
    if (listen(sd, MAXUSERS) == -1)
    {
        ClearScreen();
        perror("Error while listening");
        exit(EXIT_FAILURE);
    }
    
    int income_sd[MAXUSERS];
    
    int thread_num;
    for (thread_num = 0; thread_num < MAXUSERS; ++thread_num) 
    {
        if (server_enabled)
        {
            income_sd[thread_num] = accept(sd, 0, 0);
            
            printf("\n\nNew player connected\n");
            fflush(stdout);
            
            pthread_create(&(thread[thread_num]), 0, establishConnection, &(income_sd[thread_num]));
        }
        else
        {
            printf("\n\nServer was disabled...\n");
            fflush(stdout);
            break;
        }
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


void *ManageCommands(void *arg)
{
    char str[MAX_MES_LEN];
    
    while (1)
    {
        scanf("%s", str);
        if (!strcmp(str, "quit"))
        {            
            ClearScreen();
            printf("Server has been disabled\n");
            exit(0);
        }
        else
        {
            printf("Stop inserting shit, please.\n");
        }
    }
    
    return NULL;
}


int main() 
{
    ObtainSignals();
    srand(time(0));
    system("clear");
    numberOfPlayers = 0;
    server_enabled = 1;
    unlink(SOCKNAME);
    
    
    int i;
    for (i = 0; i < MAXUSERS; i++)
    {
        userOp[i] = i;
        userGame[i] = -1;
        users_available[i] = 0;
        games_available[i] = 0;
        left[i] = 0;
    }
    
    pthread_t connection_thread;
    pthread_t command_thread;
    
    pthread_create(&connection_thread, 0, manageConnections, 0);
    pthread_create(&command_thread, 0, ManageCommands, 0);
    
    pthread_join(connection_thread, 0);
    pthread_join(command_thread, 0);
    
    
    return 0;
}
