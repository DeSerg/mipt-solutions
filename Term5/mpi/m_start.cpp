#include "support.h"

extern int K, N, M;
extern Status status;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;

extern int **cur_table;
extern int **cur_table_new;
extern int **init_table;

const double probability = 0.5;

string filename = "/home/deserg/git/mipt-solutions/Term5/build-LifeTheGame-Desktop-Debug/table.csv";

int getInitType() {
    cout << "Please, choose the way of table initialization (type \"1\" or \"2\"):" << endl;
    cout << "\t1: Initial table is loaded from CSV file" << endl;
    cout << "\t2: Initial table is random" << endl;

    int choice;
    cin >> choice;
    while (choice != 1 && choice != 2) {
        cout << "Sorry, there is no such option number!" << endl;
        cout << "Please, choose the way of initialization (type \"1\" or \"2\"):" << endl;
        cout << "\t1: Initial table is loaded from CSV file" << endl;
        cout << "\t2: Initial table is random" << endl;
        cin >> choice;

    }
    
    return choice;
}

void fillTable(int N, int M) {
    
//    cout << "PROBABLITY " << 100 * probability << endl;
    
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < M; j++) {
            int type;
            type = rand() % 100;
//            cout << "Table generation: " << i << ":" << j << ": ";
            if (type < 100 * probability) {
//                cout << "live" << endl;
                init_table[i][j] = live;
            } else {
//                cout << "dead" << endl;
                init_table[i][j] = dead;
            }
        }
}

void getTable() {
    
    cout << "Please, enter table's height:" << endl;
    cin >> N;
    cout << "Please, enter table's width:" << endl;
    cin >> M;
    
    fillTable(N, M);
    
}

void trim(string &str) {
    std::stringstream trimmer;
    trimmer << str;
    str.clear();
    trimmer >> str;
}

int getCSVTable() {
    
    ifstream file(filename);
    string line;
    string token;
    
    if (file.is_open()) {
        
        
        init_table.clear();
        int width = 0;
        
        while (getline(file,line)) {
            
            init_table.push_back(vector<Cell>());
            
            istringstream ss(line);
            
            while(getline(ss, token, ',')) {
                int cell;
                trim(token);
                if (token == "1") {
                    cell = live;
                } else {
                    cell = dead;
                }
                init_table[init_table.size() - 1].push_back(cell);
            }
            
            int cur_width = init_table[init_table.size() - 1].size();
            if (width == 0) {
                width = cur_width;
                M = width;
            } else if (cur_width != width) {
                cout << "Table has lines with different size" << endl;
                return 1;
            }
            
        }
        
        N = init_table.size();
        file.close();
        
        return 0;
    } else {
        cout << "Something is wrong while reading the file..." << endl;
        cout << "Table will be generated randomly" << endl;
        return 1;
    }
    
    
    
    
}

void startMethod() {
    
    
//        Старт с заданным начальным распределением (см далее) и количеством
//        потоков K
//        Начальное распределение может быть задано в двух вариантах (нужно
//        реализовать оба):
//        а)файл в формате CSV (https://ru.wikipedia.org/wiki/CSV)
//        б)задаются лишь размеры NxM, поле генерируется случайным образом
    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int ready = 1;
    for (int i = 1; i <= size; ++i) {
        MPI_Send(&ready, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    
}
