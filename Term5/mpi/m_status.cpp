#include "support.h"

extern int K, N, M;
extern Status status;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern Field init_table;

extern int world_rank, world_size;
extern int work_rank, work_size;

void statusMethod() {
//        STATUS Отображение статуса текущей итерации (на которой остановились,
//        после старта -  отображение начального распределения)
    
    if (status == before_run) {
        cerr << "Game hasn't started yet" << endl;
    } else if (status == running) {
        cerr << "Game is running" << endl;
    } else if (status == stopped) {
        cerr << "Game is stopped" << endl;
    }
    
    drawTable(init_table);
    
}
