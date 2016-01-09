#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<pthread_t> thread_ref;
extern Status status;
extern int signaled;

void stopMethod() {
//        Досрочная остановка вычислений:
//        потоки-worker'ы приостанавливаются, вычислив текущую итерации и на консоли
//        выводится ее номер (с возможностью дальнейшей печати с помощью STATUS) 
    
    if (status == stopped) {
        cout << "Sorry, the game is already stopped..." << endl;
        return;
    }
    
    #pragma omp critical (signaled)
    {
        signaled = 1;
        status = stopped;
    }
}

