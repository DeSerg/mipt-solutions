#include "support.h"

extern int K, N, M;
extern Status status;

string toLower(string str) {
    string new_str = "";
    locale loc;
    for (size_t i = 0; i < str.size(); ++i) {
        new_str += std::tolower(str[i], loc);
    }
    
    return new_str;
}


void drawTable(Field &table, bool shortened) {
       
    int first_line, last_line;

    if (shortened) {
        first_line = 1;
        last_line = table.n() - 1;
    } else {
        first_line = 0;
        last_line = table.n();
    }
    
    for (int i = first_line; i < last_line; i++) {
        for (int j = 0; j < table.m(); j++) {
            if (table[i][j] == live) {
                cerr << "+";
            } else {
                cerr << ".";
            }
        }
        cerr << endl;               
    }
    cerr << endl;
    
}

int getCellNum(int i, int j) {
    return i * M + j + 1;
}

void getCellCoords(int num, int &i, int &j) {
    
    i = (num - 1) / M;
    j = num - i * M - 1;
    
}


Field::Field() : data(NULL) {}

Field::Field(int n, int m) :
    _n(n), _m(m), data(new char[n * m]) {}

Field::Field(const Field &other) :
    _n(other._n), _m(other._m), data(new char[_n * _m]) {
    for (int i = 0; i < _n*_m; ++i) {
        data[i] = other.data[i];
    }
}

Field::~Field() { delete[] data; }

Field& Field::operator=(Field other) {
    swap(*this, other);
    return *this;
}

void swap(Field &larg, Field &rarg) {
    using std::swap;
    swap(larg._n, rarg._n);
    swap(larg._m, rarg._m);
    swap(larg.data, rarg.data);
}

char *Field::operator[](const int i) { return &data[i * _m]; }
