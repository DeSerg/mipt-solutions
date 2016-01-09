#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> table;
extern vector<pthread_t> thread_ref;
extern Status status;

string toLower(string str) {
    string new_str = "";
    locale loc;
    for (size_t i = 0; i < str.size(); ++i) {
        new_str += std::tolower(str[i], loc);
    }
    
    return new_str;
}


void drawTable(vector<vector<Cell>> &table) {
    
    if ((int)table.size() < N) {
        cout << "Table size is invalid...";
        return;
    }

    for (int i = 0; i < N; i++) {
        
        if ((int)table[i].size() < M) {
            cout << "Table size is invalid...";
            return;
        }
        for (int j = 0; j < M; j++) {
            if (table[i][j] == live) {
                cout << "+";
            } else {
                cout << ".";
            }
        }
        cout << endl;               
    }
    cout << endl;
}

int getCellNum(int i, int j) {
    return i * M + j + 1;
}

void getCellCoords(int num, int &i, int &j) {
    
    i = (num - 1) / M;
    j = num - i * M - 1;
    
}
