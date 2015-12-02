#include "worker.h"

extern int K, N, M, Y;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<vector<Cell>> cur_table_new;
extern vector<vector<pthread_mutex_t>> mutex_table;

extern vector<pthread_t> thread_ref;
extern Status status;
extern int iter_cur;
extern int iter_amount;
extern int thread_iter_count;
extern pthread_mutex_t thread_count_m;
extern pthread_mutex_t condition_m;
extern pthread_cond_t condition_var;
extern vector<Data> thread_data;

int di[8] = {1, 1, 1, 0, -1, -1, -1, 0};
int dj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
int signaled;
pthread_mutex_t signaled_m;
pthread_mutex_t signaled_handler_m;
pthread_mutex_t status_m;

void obtainCell(int i, int j) {
    
    int live_sum = 0;
    
    for (int p = 0; p < 8; ++p) {
        
        int i_cur = i + di[p];
        int j_cur = j + dj[p];
        
        if (i_cur < 0 || i_cur >= N || j_cur < 0 || j_cur >= M) {
            continue;
        }
        
        pthread_mutex_lock(&mutex_table[i_cur][j_cur]);
        
        if (cur_table[i_cur][j_cur] == live) {
            ++live_sum;
        }
        
        pthread_mutex_unlock(&mutex_table[i_cur][j_cur]);
        
    }
    
    
    pthread_mutex_lock(&mutex_table[i][j]);
    Cell cell = cur_table[i][j];
    pthread_mutex_unlock(&mutex_table[i][j]);
    
    Cell new_cell = live;
    if ((cell == live && (live_sum < 2 || live_sum > 3)) ||
            (cell == dead && live_sum != 3)) 
    {
        new_cell = dead;
    }
    cur_table_new[i][j] = new_cell;    
}

void performIteration(int begin_num, int cell_amount) {
    
    int i_begin, j_begin, i_end, j_end;
    getCellCoords(begin_num, i_begin, j_begin);
    getCellCoords(begin_num + cell_amount - 1, i_end, j_end);
    
    for (int j = j_begin; j < M; ++j) {
        obtainCell(i_begin, j);
    }
    
    for (int i = i_begin + 1; i < i_end; ++i) 
        for (int j = 0; j < M; ++j) {
            obtainCell(i, j);
        }
    
    for (int j = 0; j <= j_end; ++j) {
        obtainCell(i_end, j);
    }
    
}

void sigHandler(int sigNum) {
    
    cout << "I AM BEING SIGNALLED!" << endl;
    pthread_mutex_lock(&signaled_handler_m);
    signaled = 1;
    cout << "SIGNALED IS NOW " << signaled << endl;
    pthread_mutex_unlock(&signaled_handler_m);
    
}

void *startWork(void *raw_data) {
    
    signaled_m = PTHREAD_MUTEX_INITIALIZER;
    signaled_handler_m = PTHREAD_MUTEX_INITIALIZER;
    status_m = PTHREAD_MUTEX_INITIALIZER;
    signaled = 0; 
    signal(SIGUSR1, sigHandler);
    
    Data *data = (Data *) raw_data;
    int begin_num = data->begin_num;
    int cell_amount = data->cell_amount;
    int thread_id = data->id;
    
//    cout << "Thread #" << thread_id << " is working!" << endl;
            
    for (int i = 1; i <= Y; ++i) {
        
//        cout << "#" << thread_id << ": iteration " << i << endl;
        performIteration(begin_num, cell_amount);
        
        pthread_mutex_lock(&thread_count_m);
        thread_iter_count++;
        int thread_count_local = thread_iter_count;
        pthread_mutex_unlock(&thread_count_m);
        
        pthread_mutex_lock(&condition_m);
        
        if (thread_count_local == K) {
            cout << "#" << thread_id << " is the last in iteration " << i << endl;
            thread_iter_count = 0;
            swap(cur_table, cur_table_new);
//            system("clear");
            drawTable(cur_table);
            usleep(1e6);
            pthread_cond_broadcast(&condition_var);
            
            if (i == Y) {
                cout << "Running finished!" << endl;
                drawTable(cur_table);
                status = stopped;
            }
            
        } else {
//            cout << "#" << thread_id << " is waiting..." << endl;
            pthread_cond_wait(&condition_var, &condition_m);
        }
        
//        cout << "MY ASS IS #" << thread_id << endl;
        pthread_mutex_unlock(&condition_m);
        
        pthread_mutex_lock(&signaled_m);
        cout << "Sig val: " << signaled << endl;
//        usleep(1e6);
        if (signaled == 1) {
            cout << "MY NAME IS #" << thread_id << endl;
            pthread_mutex_lock(&status_m);
            status = stopped;
            pthread_mutex_unlock(&status_m);            
            cout << "Thread #" << thread_id << " stopped at iteration " << i << endl;
            break;
        }
        pthread_mutex_unlock(&signaled_m);
        
    }
    
    
    return NULL;
}
