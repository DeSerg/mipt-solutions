//
// Created by deserg on 18/11/2018.
//

#ifndef FILE_SYSTEM_SERVER_H
#define FILE_SYSTEM_SERVER_H

#include <ntsid.h>

#define MAXUSERS 1

short users_available[MAXUSERS];
pthread_mutex_t usnum_mutex;


#endif //FILE_SYSTEM_SERVER_H
