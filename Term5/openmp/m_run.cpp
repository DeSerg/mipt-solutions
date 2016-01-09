#include "support.h"

extern int K, N, M, Y;
extern vector<vector<Cell>> cur_table;
extern vector<vector<Cell>> cur_table_new;
extern Status status;

int iter_cur;
int thread_iter_count;

void runMethod() {
//        Выполнение Y итераций
    
    if (status == running) {
        cout << "Sorry, the game is already running..." << endl;
        return;
    }
    
    K = min(N * M, K);
    iter_cur = 1;
    thread_iter_count = 0;
    
    #pragma omp parallel num_threads(K) shared(status, cur_table, cur_table_new)
    {
        startWork();
    }
    
    status = running;
        
}
