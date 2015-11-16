#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<vector<Cell>> cur_table_new;
extern vector<pthread_t> thread_ref;
extern Status status;

void calculateData(int thread_num, int iter_amount, Data &data) {
    data.table = cur_table;
    data.begin_num = N * M / K * thread_num;
    if (K == thread_num + 1) {
        data.cell_amount = N * M / K + (N * M) % K;
    } else {
        data.cell_amount = N * M / K;
    }
    data.iter_amount = iter_amount;
    data.iter_cur = 0;
}

void runMethod(int Y) {
//        Выполнение Y итераций
    thread_ref.resize(K);
    Data data = Data();
    for (int i = 0; i < K; i++) {
        calculateData(i, Y, data);
        pthread_create(&thread_ref[i], 0, &gameIteration, &data);
    }
    
}
