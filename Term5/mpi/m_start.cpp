#include "support.h"

extern int N, M;
extern Status status;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int **init_table;

extern int world_rank, world_size;

const double probability = 0.5;

string filename = "/home/serg/Documents/Programming/Parallel/mpi/table.csv";

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
                init_table[i][j] = live;
            } else {
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
            
            inp_vector.push_back(vector<int>());
            
            istringstream ss(line);
            
            while(getline(ss, token, ',')) {
                int cell;
                trim(token);
                if (token == "1") {
                    cell = live;
                } else {
                    cell = dead;
                }
                inp_vector[inp_vector.size() - 1].push_back(cell);
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
        
        if (getCSVTable() == 1) {
            getTable();
        }
        
        cout << "Here's your initial table:" << endl;
        drawTable(init_table, N, M);        
        
    } else if (choice == 2) {
        
        getTable();
        cout << "Here's your initial table:" << endl;
        drawTable(init_table, N, M);        
    } else {
        cout << "Wrong choice while starting..." << endl;
    }
        
    int count = world_size - 1;
    int integral = N / count;
    int last = N / count + N % count;
    cout << "integral: " << integral << endl;
    cout << "last: " << last << endl;
    
    if (N < count) { 
        cerr << "Too many processes:)" << endl;
        exit(0);
    }
    
    
    MPI_Bcast(&M, 1, MPI_INT, MPI_ROOT, intercomm);
    
    //processes 1 .. count - 2
    int work_N;
    for (int i = 1; i < count - 1; ++i) {
        work_N = integral + 2;
        MPI_Send(&work_N, 1, MPI_INT, i, data_tag, intercomm);
        MPI_Send(&(init_table[(i - 1) * integral][0]), work_N * M, MPI_INT, i, data_tag, intercomm);
    }

    //process 0
    work_N = integral + 1;
    MPI_Send(&work_N, 1, MPI_INT, 0, data_tag, intercomm);
    MPI_Send(&(init_table[0][0]), work_N * M, MPI_INT, 0, data_tag, intercomm);
    
    //process count - 1
    work_N = last + 1;
    MPI_Send(&work_N, 1, MPI_INT, count - 1, data_tag, intercomm);
    MPI_Send(&(init_table[N - last - 1][0]), work_N * M, MPI_INT, count - 1, data_tag, intercomm);
    
    
}
