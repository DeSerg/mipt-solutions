#include "support.h"

extern int K, N, M;
extern Status status;
extern int signaled;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int world_rank, world_size;
extern int work_rank, work_size;

extern int **init_table;

void stopMethod() {
//        Досрочная остановка вычислений:
//        потоки-worker'ы приостанавливаются, вычислив текущую итерации и на консоли
//        выводится ее номер (с возможностью дальнейшей печати с помощью STATUS) 
    
    
    int count = world_size - 1;
    int integral = N / count;
    int last = N / count + N % count;
    
    int k = 0;
    for (int i = 0; i < count; ++i) {
        MPI_Isend(&k, 1, MPI_INT, i, stop_tag, intercomm, NULL);
    }
    
    MPI_Recv(&(init_table[0][0]), integral * M, MPI_INT, 0, data_tag, intercomm, MPI_STATUS_IGNORE);
    MPI_Recv(&(init_table[count - last][0]), last * M, MPI_INT, 0, data_tag, intercomm, MPI_STATUS_IGNORE);
    
    for (int i = 1; i < count - 1; ++i) {
        MPI_Recv(&(init_table[i * integral][0]), integral * M, MPI_INT, 0, data_tag, intercomm, MPI_STATUS_IGNORE);
    }
    
    status = stopped;
}
