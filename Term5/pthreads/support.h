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

using namespace std;

enum Cell {
    live, dead
};

struct Data {
    int begin_num;
    int cell_amount;
    int id;
};

enum Status {
    running, stopped, on_quit
};

string toLower(string str);

void drawTable(vector<vector<Cell>> &table);

int getCellNum(int i, int j);

void getCellCoords(int num, int &i, int &j);

void *startWork(void *data);

void startMethod();
void statusMethod();
void runMethod();
void stopMethod();
void quitMethod();
void helpMethod();
