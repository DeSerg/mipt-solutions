#include "worker.h"

extern int K, N, M, Y;
extern vector<vector<Cell>> cur_table;
extern vector<vector<Cell>> cur_table_new;

extern Status status;
extern int signaled;

int di[8] = {1, 1, 1, 0, -1, -1, -1, 0};
int dj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

void obtainCell(int i, int j) {
    
    int live_sum = 0;
    
    for (int p = 0; p < 8; ++p) {
        
        int i_cur = i + di[p];
        int j_cur = j + dj[p];
        
        if (i_cur < 0 || i_cur >= N || j_cur < 0 || j_cur >= M) {
            continue;
        }
        
        if (cur_table[i_cur][j_cur] == live) {
            ++live_sum;
        }
                
    }
    
    Cell cell;
    #pragma omp critical (table_change)
    {
         cell = cur_table[i][j];
    }
    
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

void startWork() {
    
    signaled = 0; 
    
    int thread_id = omp_get_thread_num() + 1;
    int cell_amount;
    int begin_num = N * M / K * (thread_id - 1) + 1;
    if (K == thread_id) {
        cell_amount = N * M / K + (N * M) % K;
    } else {
        cell_amount = N * M / K;
    }

        
    for (int i = 1; i <= Y && signaled == 0; ++i) {
        
//        cout << "#" << thread_id << ": iteration " << i << endl;
        performIteration(begin_num, cell_amount);
        
        #pragma omp barrier
        
        if (thread_id == K) {
            swap(cur_table, cur_table_new);
            cerr << "Iteration " << i << " finished" << endl;
            drawTable(cur_table);
            usleep(1e6);            
            if (i == Y) {
                cout << "Running finished!" << endl;
                drawTable(cur_table);
                status = stopped;
            }    
        }
                        
        #pragma omp critical (signaled)
        {
//            cout << "Sig val: " << signaled << endl;
            if (signaled == 1) {
                cout << "Thread #" << thread_id << " stopped at iteration " << i << endl;
            }
        }
        
    }
        
}
