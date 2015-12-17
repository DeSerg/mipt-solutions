#include "support.h"

extern int K, N, M;
extern vector<vector<Cell> > init_table;
extern vector<vector<Cell> > cur_table;
extern vector<pthread_t> thread_ref;
extern Status status;

void statusMethod() {
//        STATUS Отображение статуса текущей итерации (на которой остановились,
//        после старта -  отображение начального распределения)
    if (status == running) {
        cout << "Game is running" << endl;
        drawTable(init_table);
    } else {
        cout << "Game is stopped" << endl;
        drawTable(cur_table);
    }
}
