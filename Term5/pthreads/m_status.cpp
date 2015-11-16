#include "support.h"

extern int K, N, M;
extern vector<vector<Cell>> init_table;
extern vector<vector<Cell>> cur_table;
extern vector<pthread_t> thread_ref;
extern Status status;

void statusMethod() {
//        STATUS Отображение статуса текущей итерации (на которой остановились,
//        после старта -  отображение начального распределения)
    drawTable(init_table);
}
