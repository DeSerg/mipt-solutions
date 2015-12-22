#include "support.h"

extern int K, N, M;
extern Status status;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int **cur_table;
extern int **init_table;

extern int world_rank, world_size;
extern int work_rank, work_size;

void statusMethod() {
//        STATUS Отображение статуса текущей итерации (на которой остановились,
//        после старта -  отображение начального распределения)
    
    if (status == before_run) {
        cout << "Game hasn't started yet" << endl;
    } else if (status == running) {
        cout << "Game is running" << endl;
    } else if (status == stopped) {
        cout << "Game is stopped" << endl;
    }

    drawTable(init_table, N, M);
    
}
