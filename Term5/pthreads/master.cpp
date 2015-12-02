#include "master.h"

int K, N, M, Y;
vector<vector<Cell>> cur_table;
vector<vector<Cell>> cur_table_new;
vector<vector<Cell>> init_table;
vector<pthread_t> thread_ref;
pthread_t mngr_ref;
Status status;

void readCommand() {
    
    cout << "Please, type the command:" << endl;
    
    string cmd;
    cin >> cmd; 
    cmd = toLower(cmd);
    
    cout << endl;
    if (cmd == "start") {
        startMethod();

    } else if (cmd == "status") {
        statusMethod();

    } else if (cmd == "run") {
        
        cout << "Please, insert number of interations:" << endl;
        cin >> Y;
        runMethod();
        
    } else if (cmd == "stop") {
        stopMethod();
             
    } else if (cmd == "quit") {
        quitMethod();
       
    } else if (cmd == "help") {
        helpMethod();
       
    } else {
        cout << endl << "Wrong command" << endl;
    }
    
}

void test() {
    
    N = 4;
    M = 4;
    int test_num = 15;
    
    for (int t = 0; t < test_num; ++t) {
        
        int i = rand() % N;
        int j = rand() % M;
        int num = getCellNum(i, j);
        int i_, j_;
        getCellCoords(num, i_, j_);
        cout << i << " : " << j << endl << num << endl << i_ << " : " << j << endl << endl;        
    }
    
}

int main()
{
    
    srand(time(NULL));
    status = stopped;    
    
//    test();

    cout << "HELLO!" << endl;
    startMethod();
    helpMethod();
    
    while (status != on_quit) {
        cout << "\n\n";
        readCommand();
    }
    
    cout << "Good bye.." << endl;
    return 0;
}

