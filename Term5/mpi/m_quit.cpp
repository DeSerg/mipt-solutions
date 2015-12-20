#include "support.h"

extern int K, N, M;
extern Status status;

void quitMethod() {
    
    //Завершение программы с корректной приостановкой потоков 
    
    if (status == running) {
        stopMethod();
    }
    
    status = on_quit;
    
}
