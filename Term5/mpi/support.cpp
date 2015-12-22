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


void drawTable(int **table, int N, int M) {
    
    for (int i = 0; i < N; i++) {
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

int **allocArray(int N, int M) {

    int **array;
    
    if ((array = (int **) malloc(N * sizeof(int*))) == NULL) {
        cerr << "err" << endl;
        exit(1);
    }
    
    for (int i = 0; i < N; ++i) {
      if ((array[i] = (int *) malloc(M * sizeof(int))) == NULL ) {
          cerr << "err" << endl;
          exit(1);
      }    
    }
    
    return array;
}

void deallocArray(int **array, int N) {
    
    for(int i = 0; i < N; ++i) {
        free(array[i]);
    }
    
    free(array);
}

void copyArray(int **source, int **dest, int N, int M) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            dest[i][j] = source[i][j];
        }
    }
}
