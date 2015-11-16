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

using namespace std;

enum Cell {
    live, dead
};

struct Data {
    vector<vector<Cell>> table;
    int begin_num;
    int cell_amount;
    int iter_amount;
    int iter_cur;
};

enum Status {
    before_start, in_progress, stopped, on_quit
};

string toLower(string str);

void drawTable(vector<vector<Cell>> table);

int getCellNum(int i, int j);

void getCellCoords(int num, int &i, int &j);

void *gameIteration(void *data);
