#include "support.h"

extern int K, N, M;
extern Status status;
extern int signaled;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int world_rank, world_size;
extern int work_rank, work_size;

extern Field init_table;

void stopMethod() {
//        Досрочная остановка вычислений:
//        потоки-worker'ы приостанавливаются, вычислив текущую итерации и на консоли
//        выводится ее номер (с возможностью дальнейшей печати с помощью STATUS) 
    if (status == stopped) {
        cerr << "Game is stopped already..." << endl;
        return;
    }
    vector<MPI_Request> send_req(world_size - 1);
    for (int i = 1; i < world_size; ++i) {
        MPI_Isend(NULL, 0, MPI_INT, i, stop_tag, MPI_COMM_WORLD, &(send_req[i - 1]));
    }
    
    for (int i = 0; i < world_size - 1; ++i) {
        MPI_Wait(&send_req[i], MPI_STATUS_IGNORE);
    }

}
