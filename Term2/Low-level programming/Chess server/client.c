#include "messages.h"
#include <ctype.h>

UserData user_data;
Color color;
int sd;
int login_s;
pthread_mutex_t print_mutex;


void DrawField(Disposition *d)
{
    printf(BORD"    a  b  c  d  e  f  g  h    \n");
    int i, j;
    Color color = black;
    for (i = 0; i < 8; i++)
    {
        color = (color + 1) % 2;
        for (j = 0; j < 10; j++)
        {
            if (j == 0 || j == 9)
            {
                printf(BORD" %d ", 8 - i);
            }
            else
            if (d[COORD].type == none)
            {
                if (color) 
                    printf(B_B"   ");
                else
                    printf(W_B"   ");
                color = (color + 1) % 2;
            }
            else
            if(d[COORD].color == white)
            {
                if (d[COORD].type == pawn)
                {
                    if (color)
                        printf(B_W" ♙ ");
                    else 
                        printf(W_W" ♙ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == knight)
                {
                    if (color)
                        printf(B_W" ♘ ");
                    else 
                        printf(W_W" ♘ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == bishop)
                {
                    if (color)
                        printf(B_W" ♗ ");
                    else 
                        printf(W_W" ♗ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == rook)
                {
                    if (color)
                        printf(B_W" ♖ ");
                    else 
                        printf(W_W" ♖ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == queen)
                {
                    if (color)
                        printf(B_W" ♕ ");
                    else 
                        printf(W_W" ♕ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == king)
                {
                    if (color)
                        printf(B_W" ♔ ");
                    else 
                        printf(W_W" ♔ ");
                    color = (color + 1) % 2;
                }
            }
            else
            {
                if (d[COORD].type == pawn)
                {
                    if (color)
                        printf(B_B" ♟ ");
                    else 
                        printf(W_B" ♟ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == knight)
                {
                    if (color)
                        printf(B_B" ♞ ");
                    else 
                        printf(W_B" ♞ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == bishop)
                {
                    if (color)
                        printf(B_B" ♝ ");
                    else 
                        printf(W_B" ♝ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == rook)
                {
                    if (color)
                        printf(B_B" ♜ ");
                    else 
                        printf(W_B" ♜ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == queen)
                {
                    if (color)
                        printf(B_B" ♛ ");
                    else 
                        printf(W_B" ♛ ");
                    color = (color + 1) % 2;
                }
                else
                if (d[COORD].type == king)
                {
                    if (color)
                        printf(B_B" ♚ ");
                    else 
                        printf(W_B" ♚ ");
                    color = (color + 1) % 2;
                }
            }
                    
        }               
        printf("\n");
    }
    printf(BORD"    a  b  c  d  e  f  g  h    "RES"\n");
    
}


void ClearScreen()
{
    system("clear");
}


void SomeInfo()
{
    if (color == white)
        printf("[You are playing white]\n");
    else
        printf("[You are playing black]\n");
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
    
    printf("\n\nPlease, enter your chess level (1 - 10):\n");
    pthread_mutex_lock(&print_mutex);
    scanf("%d", &user_data.ownLevel);
    pthread_mutex_unlock(&print_mutex);
    while (user_data.ownLevel < 1 || user_data.ownLevel > 10)
    {
        ClearScreen();
        printf("Wrong level!\n");
        printf("Please, enter your chess level (1 - 10):\n");
        scanf("%d", &user_data.ownLevel);
    }
    
    printf("\n\nPlease, enter desired chess level of your opponent (1 - 10):\n");
    pthread_mutex_lock(&print_mutex);
    scanf("%d", &user_data.desiredLevel);
    pthread_mutex_unlock(&print_mutex);
    while (user_data.desiredLevel < 1 || user_data.desiredLevel > 10)
    {
        ClearScreen();
        printf("Wrong level!\n");
        printf("Please, enter desired chess level of your opponent (1 - 10):\n");
        scanf("%d", &user_data.desiredLevel);
    }
    
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
            printf("\n\nYour game begins!\n");
            printf("You will play with %s\n", str);
            
            getMessage(sd, &m);
            color = *(Color *)m.data;
            if (color == white)
                printf("Your color is white!\n");
            else
                printf("Your color is black!\n");
            
            printf("\n\nInsert 'help' to see some useful information\n");
        }
        else
        {
            ClearScreen();
            printf("Sorry, there are no free slots for playing chess...\n");
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


void UserHelp()
{    
    ClearScreen();
    printf(
                "Weclome to Serg's chess server!\n"
                "\n"
                "Here are some useful commands:\n"
                "\t'help' - to see help\n"
                "\t'quit' - to quit game\n"
                "\t'turn' - to make your turn (for example, 'e2e4')\n"  
                "\t'disp' - to see the figures disposition\n" 
                "\n"
                "Have a nice game:)\n"
                "\n"
                );
}


void UserDisposition(int clear)
{
    if (clear)
        ClearScreen();
    
    Disposition *d;
    MessageType m = composeMessage(disposition, 0, NULL);
    sendMessage(sd, &m);
    getMessage(sd, &m);
    
    int result_val = *(int *)m.data;
    if (result_val)
    {
        getMessage(sd, &m);        
        d = (Disposition *)m.data;
    }
    else
    {
        printf("Sorry, but your game is over...\n");
        UserLogout(0);
        return;
    }

    SomeInfo();
    DrawField(d);  
}


void ReadDisposition(Disposition **d)
{
    MessageType m = composeMessage(disposition, 0, NULL);
    sendMessage(sd, &m);
    getMessage(sd, &m);
    
    
    int result_val = *(int *)m.data;
    if (result_val)
    {
        getMessage(sd, &m);
        *d = (Disposition *)m.data;        
    }
    else
    {
        printf("Sorry, but your game is over...\n");
        UserLogout(0);
        return;
    }
}


int WrongTurn(Disposition *d)
{
    SomeInfo();
    DrawField(d);
    char turn_str[MAX_MES_LEN];
    printf("\nInsert your turn, please\n");
    pthread_mutex_lock(&print_mutex);
    scanf("%s", turn_str);
    pthread_mutex_unlock(&print_mutex);
        
    MessageType m = composeMessage(turn, sizeof(turn_str), turn_str);
    sendMessage(sd, &m);
    getMessage(sd, &m);

    int result_val = *(int *)m.data;
    if (result_val == 0)
    {
        getMessage(sd, &m);
        
        ClearScreen();
        printf("Sorry, but your turn is incorrect:\n");
        printf("%s\n", (char *)m.data);                
        return 0;
    }
    if (result_val == 1)
    {
        ClearScreen();
        printf("Not bad! Now your opponent will make a turn.\n");
        return 1;
    }
    else
    {
        ClearScreen();
        printf("Sorry, but your game is over...\n");
        UserLogout(0);
        return 1;
    }    
}


void UserTurn(Disposition *d)
{
    
    char turn_str[MAX_MES_LEN];
    printf("Insert your turn, please\n");
    pthread_mutex_lock(&print_mutex);
    scanf("%s", turn_str);
    pthread_mutex_unlock(&print_mutex);
    
    MessageType m = composeMessage(turn, sizeof(turn_str), turn_str);
    sendMessage(sd, &m);
    getMessage(sd, &m);
    
    int result_val = *(int *)m.data;
    if (result_val == 0)
    {
        getMessage(sd, &m);
        
        ClearScreen();
        printf("Sorry, but your turn is incorrect:\n");
        printf("%s\n", (char *)m.data);
        
        while (1)
        {
            if (WrongTurn(d))
                break;
        }
    }
    else
    {
        ClearScreen();
        printf("Not bad! Now your opponent will make a turn.\n");
    }

    clearMessage(&m);    
}


void UserTryTurn()
{
    ClearScreen();
       
    Disposition *d;
    ReadDisposition(&d);
    
    MessageType m = composeMessage(try_turn, 0, NULL);
    sendMessage(sd, &m);
    getMessage(sd, &m);
    
    int result_val = *(int *)m.data;
    if (result_val == 0)
    {
        printf("Sorry, but your game is over...\n");
        UserLogout(0);
        return;
    }
    else
    if (result_val == 1)
    {
        printf("Sorry, but your turn is not now.\n");
        UserDisposition(0);
        return;
    }
    else
    if (result_val == 2)
    {
        SomeInfo();
        DrawField(d);
        UserTurn(d);
        UserDisposition(0);
    }

    clearMessage(&m);    

}


void handler(int sigVal)
{
    if (sigVal == SIGINT)
    {
        ClearScreen();
        if (login_s)
            UserLogout(1);
        printf("Goodbye! Have a nice day:)\n");
        exit(0);
    }
}


void ObtainSignals()
{
    signal(SIGINT, &handler);
}


void *ManageCommands(void *arg)
{
    char command[MAX_MES_LEN];
    
    while (1)
    {

        scanf("%s", command);
        
        if (!strcmp(command, "help"))
            UserHelp();
        else
        if (!strcmp(command, "quit"))
            UserLogout(1); 
        else
        if (!strcmp(command, "turn"))
            UserTryTurn();  
        else
        if (!strcmp(command, "disp"))
            UserDisposition(1);
        else
        if (!strcmp(command, "info"))
            UserTryTurn();  
        else
        {
            printf("Stop inserting shit, please.\n");
        }
    }
    
    return NULL;
}


int main()
{
    ClearScreen();
    printf("Hello! Welcome to Serg's chess!\n");
        
    ObtainSignals();
    
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
