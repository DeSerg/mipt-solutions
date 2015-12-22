#include "worker.h"

extern int N, M, Y;

int di[8] = {1, 1, 1, 0, -1, -1, -1, 0};
int dj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
int signaled;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int world_rank, world_size;
extern int work_rank, work_size;

int **worker_table;
int **worker_table_new;

int quit_flag, stop_flag, run_flag;


void obtainCell(int i, int j) {
    
    int live_sum = 0;
    
    for (int p = 0; p < 8; ++p) {
        
        int i_cur = i + di[p];
        int j_cur = j + dj[p];
        
        if (i_cur < 0 || i_cur >= N || j_cur < 0 || j_cur >= M) {
            continue;
        }
        
        if (worker_table[i_cur][j_cur] == live) {
            ++live_sum;
        }
    }
    
    int cell = worker_table[i][j];
    
    int new_cell = live;
    if ((cell == live && (live_sum < 2 || live_sum > 3)) ||
            (cell == dead && live_sum != 3)) 
    {
        new_cell = dead;
    }
    worker_table_new[i][j] = new_cell;    
    
}

void performIteration() {
       
    memcpy(&(worker_table[0][0]), &(worker_table_new[0][0]), M * sizeof(int));
    memcpy(&(worker_table[N][0]), &(worker_table_new[N][0]), M * sizeof(int));
    
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 0; j < M; ++j) {
            obtainCell(i, j);
        }
    }
    
}

void startGame() {
    
    MPI_Iprobe(0, stop_tag, intercomm, &stop_flag, NULL);
    MPI_Irecv(&Y, 1, MPI_INT, 0, run_tag, intercomm, NULL);
    
    for (int i = 0; i < Y && !stop_tag; ++i) {
        performIteration();
        
        swap(worker_table, worker_table_new);

        MPI_Request send1, send2, recv1, recv2;
        if (work_rank != 0) {
            MPI_Isend(&(worker_table_new[0][0]), M, MPI_INT, work_rank - 1, (work_rank + 1) * 100 + 1, work_comm, &send1);
            MPI_Irecv(&(worker_table[0][0]), M, MPI_INT, work_rank - 1, work_rank * 100, work_comm, &recv1);
        }
        if (work_rank != work_size - 1) {
            MPI_Isend(&(worker_table_new[N-1][0]), M, MPI_INT, work_rank + 1, (work_rank + 1) * 100, work_comm, &send2);
            MPI_Irecv(&(worker_table[N-1][0]), M, MPI_INT, work_rank + 1, (work_rank + 2) * 100 + 1, work_comm, &recv2);
        }
        
        if (work_rank != 0) {
            MPI_Wait(&send1, MPI_STATUS_IGNORE);
            MPI_Wait(&recv1, MPI_STATUS_IGNORE);
        }
        if (work_rank != work_size - 1) {
            MPI_Wait(&send2, MPI_STATUS_IGNORE);
            MPI_Wait(&recv2, MPI_STATUS_IGNORE);
        }
        
        MPI_Barrier(work_comm);
        MPI_Iprobe(0, stop_tag, intercomm, &stop_flag, NULL);
        
    }
    
    if (work_rank != 0 && work_rank != work_size - 1) {
        MPI_Send(&(worker_table[1][0]), (N - 2) * M, MPI_INT, 0, data_tag, intercomm);
    }
    if (work_rank == 0) {
        MPI_Send(&(worker_table[0][0]), (N - 1) * M, MPI_INT, 0, data_tag, intercomm);
    }
    if (work_rank == work_size - 1) {
        MPI_Send(&(worker_table[1][0]), (N - 1) * M, MPI_INT, 0, data_tag, intercomm);
    }
    
}

void work() {
    
    MPI_Bcast(&M, 1, MPI_INT, 0, intercomm);
    MPI_Recv(&N, 1, MPI_INT, 0, data_tag, intercomm, MPI_STATUS_IGNORE);
        
    worker_table = allocArray(N, M);
    worker_table_new = allocArray(N, M);
    
    MPI_Recv(&(worker_table[0][0]), N * M, MPI_INT, 0, data_tag, intercomm, MPI_STATUS_IGNORE);
    
    for (int i = 0; i < work_size; ++i) {
        
        if (work_rank == i) {
            cerr << "#" << work_rank << ": " << N << "x" << M << endl;
            drawTable(worker_table, N, M);
        }
        
        MPI_Barrier(work_comm);
    }
    
    MPI_Iprobe(0, quit_tag, intercomm, &quit_flag, NULL);
    
    if (quit_flag) {
        return;
    }
    
    while (!quit_flag) {
        
        MPI_Iprobe(0, run_tag, intercomm, &run_flag, NULL);
        
        if (run_flag) {
            startGame();
        }
        
        MPI_Iprobe(0, quit_tag, intercomm, &quit_flag, NULL);
        
    }
    
    deallocArray(worker_table, N);
    deallocArray(worker_table_new, N);
}
