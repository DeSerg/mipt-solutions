#ifndef MESSAGES_HH
#define MESSAGES_HH
#include <sys/socket.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>

#define MAXNAMELENGTH 50
#define MAX_IP_LENGTH 20
#define MAX_MES_LEN 50
#define port 4000

#define B_B "\x1b[1;30;41m"
#define W_B "\x1b[1;30;42m"
#define B_W "\x1b[1;37;41m"
#define W_W "\x1b[1;37;42m"
#define BORD "\x1b[1;36;40m" 
#define RES "\x1b[0m" 

#define COORD (7 - i) * 8 + j - 1


typedef struct
{
    char name[MAXNAMELENGTH];//статический массив, чтобы можно было пользоваться sizeof
    int userID;

} UserData;


//Общий тип для сообщений
typedef struct 
{
    enum Type 
    {
        login, 
        logout,
        command,
        result,
        color_t} type;
    size_t size;//длина data
    void* data;
} MessageType;


//Создать сообщение
MessageType composeMessage(enum Type type, size_t size, void* data);

//Получить сообщение
int getMessage(int sd, MessageType* m);


//Отправить сообщение
int sendMessage(int sd, MessageType* m);

void clearMessage(MessageType *m);

void sendText(int sd, char *str);

#define SOCKNAME "mysocket"

#endif
