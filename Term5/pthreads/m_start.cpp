#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<vector<Cell>> cur_table_new;
extern vector<pthread_t> thread_ref;
extern Status status;

const double probability = 0.5;

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
    cur_table.assign(init_table.begin(), init_table.end());
    cur_table_new.assign(init_table.begin(), init_table.end());
}


void startMethod() {
    
    
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
        //not done!!
        cout << "Sorry, not implemented yet.." << endl;
    } else if (choice == 2) {
        cout << "Please, enter table's height:" << endl;
        cin >> N;
        cout << "Please, enter table's width:" << endl;
        cin >> M;
        
        fillTable(N, M);
        cout << "Here's your initial table:" << endl;
        drawTable(init_table);        
    } else {
        cout << "Wrong choice while starting..." << endl;
    }
    status = stopped;
}
