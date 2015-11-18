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
    before_start, in_progress, stopped, on_quit
};

string toLower(string str);

void drawTable(vector<vector<Cell>> &table);

int getCellNum(int i, int j);

void getCellCoords(int num, int &i, int &j);

void *startWork(void *data);
