#include "support.h"

extern int K, N, M;
extern Status status;
extern int signaled;

extern MPI_Comm work_comm;
extern MPI_Comm master_comm;
extern MPI_Comm intercomm;

extern int world_rank, world_size;
extern int work_rank, work_size;

void stopMethod() {
//        Досрочная остановка вычислений:
//        потоки-worker'ы приостанавливаются, вычислив текущую итерации и на консоли
//        выводится ее номер (с возможностью дальнейшей печати с помощью STATUS) 
    
        
}

