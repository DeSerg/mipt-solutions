#include "support.h"

extern int K, N, M, Y;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<vector<Cell>> cur_table_new;
extern vector<pthread_t> thread_ref;
extern Status status;

int iter_cur;
int thread_iter_count;
pthread_mutex_t thread_count_m;
pthread_mutex_t condition_m;
vector<vector<pthread_mutex_t>> mutex_table;
Data *data;
pthread_cond_t condition_var;
vector<Data> thread_data;

void calculateData(int thread_num, Data &data) {
    data.id = thread_num + 1;
    data.begin_num = N * M / K * thread_num + 1;
    if (K == thread_num + 1) {
        data.cell_amount = N * M / K + (N * M) % K;
    } else {
        data.cell_amount = N * M / K;
    }
}

void runMethod() {
//        Выполнение Y итераций
    
    if (status == running) {
        cout << "Sorry, the game is already running..." << endl;
        return;
    }
    
    K = min(N * M, K);
    thread_ref.resize(K);
    data = new Data[K];
    iter_cur = 1;
    thread_iter_count = 0;
    thread_count_m = PTHREAD_MUTEX_INITIALIZER;
    condition_m = PTHREAD_MUTEX_INITIALIZER;
    
    mutex_table.resize(N);
    for (int i = 0; i < N; i++) {
        mutex_table[i].assign(M, PTHREAD_MUTEX_INITIALIZER);
                
    }
    
    for (int i = 0; i < K; i++) {
        calculateData(i, data[i]);
        pthread_create(&thread_ref[i], 0, &startWork, &data[i]);
    }
    
    status = running;
    
}
