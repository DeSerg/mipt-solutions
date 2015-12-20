#include "support.h"

extern int K, N, M;
extern Status status;

void helpMethod() {
//        STATUS Отображение статуса текущей итерации (на которой остановились,
//        после старта -  отображение начального распределения)
    cout << "Program supports following commands:" << endl;
    cout << "\"STATUS\" - Initial disposition, if game is in running," 
            " disposition where the game stopped otherwise" << endl; 
    cout << "\"RUN\" - Run the game" << endl;
    cout << "\"STOP\" - Stop the game" << endl;
    cout << "\"QUIT\" - Quit the game" << endl;
    cout << "\"HELP\" - Remind commands" << endl;
}

