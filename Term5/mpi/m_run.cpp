#include "support.h"

extern int K, N, M, Y;
extern Status status;

extern int world_rank, world_size;
extern int work_rank, work_size;

extern MPI_Comm intercomm;

void runMethod() {
//        Выполнение Y итераций
    
    vector<MPI_Request> send_req(world_size - 1);
    for (int i = 1; i < world_size; ++i) {
            MPI_Isend(&Y, 1, MPI_INT, i, run_tag, MPI_COMM_WORLD, &(send_req[i - 1]));
    }
    
    for (int i = 0; i < world_size - 1; ++i) {
        MPI_Wait(&send_req[i], MPI_STATUS_IGNORE);
    }
    
    status = running;
}
