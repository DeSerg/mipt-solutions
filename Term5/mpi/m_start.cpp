#include "support.h"

extern int N, M;
extern Status status;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int **cur_table;
extern int **init_table;

extern int world_rank, world_size;

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
    
    init_table = allocArray(N, M);
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
    
    vector<vector<int>> inp_vector;
    
    if (file.is_open()) {
        
        
        inp_vector.clear();
        int width = 0;
        
        while (getline(file,line)) {
            
            inp_vector.push_back(vector<Cell>());
            
            istringstream ss(line);
            
            while(getline(ss, token, ',')) {
                int cell;
                trim(token);
                if (token == "1") {
                    cell = live;
                } else {
                    cell = dead;
                }
                inp_vector[init_table.size() - 1].push_back(cell);
            }
            
            int cur_width = inp_vector[inp_vector.size() - 1].size();
            if (width == 0) {
                width = cur_width;
                M = width;
            } else if (cur_width != width) {
                cout << "Table has lines with different size" << endl;
                return 1;
            }
            
        }
        
        N = inp_vector.size();
        file.close();
        
        init_table = allocArray(N, M);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                
                init_table[i][j] = inp_vector[i][j];
            }
            
        }
        
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
    int choice = getInitType();
    
    if (choice == 1) {
        getCSVTable();
    } else {
        getTable();
    }
    
    int count = world_size - 1;
    int integral = N / count;
    int last = N / count + N % count;
    
    if (N < count) { 
        cerr << "Too many processes:)" << endl;
        exit(0);
    }
    
    for (int i = 0; i < count; ++i) {
        MPI_Bcast(&N, 1, MPI_INT, 0, intercomm);
        MPI_Bcast(&M, 1, MPI_INT, 0, intercomm);
    }
    
    MPI_Send(&(init_table[0][0]), (integral + 1) * M, MPI_INT, 0, data_tag, intercomm);
    MPI_Send(&(init_table[count - last - 1][0]), (last + 1) * M, MPI_INT, 0, data_tag, intercomm);
    
    for (int i = 1; i < count - 1; ++i) {
        MPI_Send(&(init_table[(i - 1) * integral][0]), (integral + 2) * M, MPI_INT, 0, data_tag, intercomm);
    }
    
}
