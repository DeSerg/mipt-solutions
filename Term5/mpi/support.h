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
#include <string.h>

#define live 0
#define dead 1

#define data_tag 0
#define return_data_tag 5
#define run_tag 10
#define stop_tag 20
#define quit_tag 30

using namespace std;

class Field {
    int _n;
    int _m;
    char *data;
public:
    Field();
    Field(int n, int m);
    Field(const Field &other);
    ~Field();
    Field& operator=(Field other);
    friend void swap(Field &larg, Field &rarg);

    char *operator[](const int i);
    int n() { return _n; }
    int m() { return _m; }
    // char* serial();
};    

struct Data {
    int begin_num;
    int cell_amount;
    int id;
};

enum Status {
    before_run, running, stopped, on_quit
};

string toLower(string str);

void drawTable(Field &table, bool shortened = false);

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
void deallocArray(int **array, int N);
void copyArray(int **source, int **dest, int N, int M);


