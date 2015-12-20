#include "worker.h"

int K, N, M, Y, ID;

int di[8] = {1, 1, 1, 0, -1, -1, -1, 0};
int dj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
int signaled;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;

int **worker_table;
int **worker_table_new;

void obtainCell(int i, int j) {
    
}

void performIteration(int begin_num, int cell_amount) {
    
}

void work() {
    
    MPI_Comm_size(MPI_COMM_WORLD, &K);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    
    MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    MPI_Recv(&K, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    
    worker_table = allocArray(N, M);
    worker_table_new = allocArray(N, M);
    
    MPI_Recv(&(worker_table[0][0]), N * M, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    
    
    cout << "Worker #" << rank << ": I AM READY!";
        
    
}
