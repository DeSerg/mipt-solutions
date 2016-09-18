#ifndef SERVER_H
#define SERVER_H

#include "messages.h"
#define MAXUSERS 20
#define MAXGAMES 10

UserData users[MAXUSERS];

int numberOfPlayers; //её изменение обернуть мьютексом

int userOp[MAXUSERS];

int userGame[MAXUSERS];//здесь хранится соответствие пользователя и игры

Game games[MAXGAMES]; //здесь храним игры

short users_available[MAXUSERS];
short games_available[MAXGAMES];
short left[MAXUSERS];

int server_enabled;

pthread_mutex_t usnum_mutex;

#endif
