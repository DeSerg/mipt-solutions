#ifndef SUPPORT
#define SUPPORT

#endif // SUPPORT


#include <string>
#include <locale> 
#include <iostream>
#include <pthread.h>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <new>
#include <csignal>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <mpi.h>

#define live 0
#define dead 1

using namespace std;


struct Data {
    int begin_num;
    int cell_amount;
    int id;
};

enum Status {
    running, stopped, on_quit
};

string toLower(string str);

void drawTable(int **table, int N, int M);

int getCellNum(int i, int j);

void getCellCoords(int num, int &i, int &j);

void work();

void startMethod();
void statusMethod();
void runMethod();
void stopMethod();
void quitMethod();
void helpMethod();

int **allocArray(int N, int M);
void copyArray(int **source, int **dest, int N, int M);
