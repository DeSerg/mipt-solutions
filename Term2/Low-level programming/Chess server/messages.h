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
    int ownLevel; //свой уровень
    int desiredLevel; //желаемый уровень соперника
    int userID;
    
} UserData;


//Тип для ходов
typedef struct {
    unsigned short startPos;
    unsigned short endPos;
} Turn;


//Тип для результата хода (корректность)
//int, 0 - некорректный, 1 - корректный, 2 - корректный и съел фигуру


typedef enum 
{
    white,
    black
} Color;

typedef enum
{
    pawn/*пешка*/, 
    rook/*Ладья*/, 
    king, 
    bishop/*слон*/, 
    knight/*конь*/, 
    queen/*ферзь*/, 
    none
} FigureType;

//Тип для описания конкретной клетки
typedef struct {
    FigureType type;
    Color color;
} Disposition;


//Тип для игры
typedef struct {
    Disposition d[64];
    UserData white_user;
    UserData black_user;
    Color current;
} Game;
 
//Общий тип для сообщений
typedef struct 
{
    enum Type 
    {
        login, 
        logout, 
        try_turn,
        turn, 
        disposition, 
        result, 
        userlist, 
        user, 
        endoflist, 
        text, 
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
