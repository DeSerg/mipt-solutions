#include "support.h"

extern int K, N, M, Y;
extern Status status;

extern int world_rank, world_size;
extern int work_rank, work_size;

extern MPI_Comm intercomm;

void runMethod() {
//        Выполнение Y итераций
    MPI_Request send_req;
    
    for (int i = 0; i < work_size; ++i) {
            MPI_Isend(&Y, 1, MPI_INT, i, run_tag, intercomm, &send_req);
    }
    
    status = running;
}
