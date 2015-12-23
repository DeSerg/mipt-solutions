#include "support.h"

extern int K, N, M;
extern Status status;

extern int world_rank, world_size;
extern int work_rank, work_size;

extern MPI_Comm intercomm;

void quitMethod() {
    
    //Завершение программы с корректной приостановкой потоков 
    
    cerr << "master: on quit..." << endl;
    
    vector<MPI_Request> req(world_size - 1);
    for (int i = 1; i < world_size; ++i) {
        MPI_Isend(NULL, 0, MPI_INT, i, quit_tag, MPI_COMM_WORLD, &(req[i - 1]));
    }
    
    cerr << "master: waiting workers to quit..." << endl;
    for (int i = 0; i < work_size; ++i) {
        MPI_Wait(&req[i], MPI_STATUS_IGNORE);
    }
    cerr << "master: workers are quitting" << endl;
    
    if (status == running) {
        stopMethod();
    }
    
    status = on_quit;
}
