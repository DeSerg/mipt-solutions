#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<pthread_t> thread_ref;
extern Status status;

void quitMethod() {
    
    //Завершение программы с корректной приостановкой потоков 
    
    //not done yet
    status = on_quit;
    
}
