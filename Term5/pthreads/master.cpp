#include "master.h"

int K, N, M, Y;
vector<vector<Cell>> cur_table;
vector<vector<Cell>> cur_table_new;
vector<vector<Cell>> init_table;
vector<pthread_t> thread_ref;
pthread_t mngr_ref;
Status status;

void readCommand() {
    
    cout << "Please, choose the action:" << endl;
    cout << "\"STATUS\" - Initial disposition, if game is in running," 
            " disposition where the game stopped otherwise" << endl; 
    cout << "\"RUN\" - Run the game" << endl;
    cout << "\"STOP\" - Stop the game" << endl;
    cout << "\"QUIT\" - Quit the game" << endl;
    
    string cmd;
    cin >> cmd; 
    cmd = toLower(cmd);

    if (cmd == "status") {
        statusMethod();

    } else if (cmd == "run") {
        
        cout << "Please, insert number of interations:" << endl;
        cin >> Y;
        runMethod(Y);
        
    } else if (cmd == "stop") {
        stopMethod();
             
    } else if (cmd == "quit") {
        quitMethod();
       
    } else {
        cout << endl << "Wrong command" << endl;
    }
    
}

int main()
{
    srand(time(NULL));
    status = before_start;    

    //        Старт с заданным начальным распределением (см далее) и количеством
    //        потоков K
    //        Начальное распределение может быть задано в двух вариантах (нужно
    //        реализовать оба):
    //        а)файл в формате CSV (https://ru.wikipedia.org/wiki/CSV)
    //        б)задаются лишь размеры NxM, поле генерируется случайным образом
    startMethod();
    
    while (status != on_quit) {
        cout << "\n\n";
        readCommand();
    }
    return 0;
}

