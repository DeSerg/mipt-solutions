#include "support.h"

extern int K, N, M;
extern vector<vector<Cell> > init_table;
extern vector<vector<Cell> > cur_table;
extern vector<pthread_t> thread_ref;
extern Status status;

void quitMethod() {
    
    //Завершение программы с корректной приостановкой потоков 
    
    if (status == running) {
        stopMethod();
    }
    
    status = on_quit;
    
}
