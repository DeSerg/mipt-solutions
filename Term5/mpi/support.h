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
#include <mpi/mpi.h>

#define live 0
#define dead 1

#define data_tag 0
#define run_tag 10
#define stop_tag 20
#define quit_tag 30

using namespace std;


struct Data {
    int begin_num;
    int cell_amount;
    int id;
};

enum Status {
    before_run, running, stopped, on_quit
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
