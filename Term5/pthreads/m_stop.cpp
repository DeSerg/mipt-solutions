#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<pthread_t> thread_ref;
extern Status status;

void stopMethod() {
//        Досрочная остановка вычислений:
//        потоки-worker'ы приостанавливаются, вычислив текущую итерации и на консоли
//        выводится ее номер (с возможностью дальнейшей печати с помощью STATUS) 
    
    if (status == stopped) {
        cout << "Sorry, the game is already stopped..." << endl;
        return;
    }
    
    if ((int)thread_ref.size() != K) {
        cout << "Something is wrong" << endl;
        return;
    }
    
    for (int i = 0; i < K; ++i) {
        pthread_kill(thread_ref[i], SIGUSR1);
    }
        
}

