#include "support.h"

extern int K, N, M;
extern vector<vector<Cell> > init_table;
extern vector<vector<Cell> > cur_table;
extern vector<vector<Cell> > cur_table_new;
extern vector<pthread_t> thread_ref;
extern Status status;

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
    init_table.resize(N);
    for (int i = 0; i < N; i++) {
        init_table[i].resize(M);
    }
    
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

/*
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
                Cell cell;
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
*/
void startMethod() {
    
    
//        Старт с заданным начальным распределением (см далее) и количеством
//        потоков K
//        Начальное распределение может быть задано в двух вариантах (нужно
//        реализовать оба):
//        а)файл в формате CSV (https://ru.wikipedia.org/wiki/CSV)
//        б)задаются лишь размеры NxM, поле генерируется случайным образом
    
    cout << "Please, enter number of threads:" << endl;
    cin >> K;
    
    while (K <= 0) {
        cout << "Number of threads should be positive!" << endl;
        cout << "Please, enter number of threads:" << endl;
        cin >> K;        
    }
    thread_ref.resize(K);
    
    int choice = getInitType();
    
    if (choice == 1) {
        
        if (/*getCSVTable()*/1 == 1) {
            getTable();
        }
        
        cout << "Here's your initial table:" << endl;
        drawTable(init_table);        
        
    } else if (choice == 2) {
        
        getTable();
        cout << "Here's your initial table:" << endl;
        drawTable(init_table);        
    } else {
        cout << "Wrong choice while starting..." << endl;
    }
    
    cur_table.assign(init_table.begin(), init_table.end());
    cur_table_new.assign(init_table.begin(), init_table.end());
    
    status = stopped;
}
