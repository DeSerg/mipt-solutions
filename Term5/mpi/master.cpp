#include "support.h"

int K, N, M, Y;
vector<vector<Cell> > cur_table;
vector<vector<Cell> > cur_table_new;
vector<vector<Cell> > init_table;
vector<pthread_t> thread_ref;
pthread_t mngr_ref;
Status status;

MPI_Comm work_comm;
MPI_Comm master_comm;

void readCommand() {
    
    cout << "Please, type the command:" << endl;
    
    string cmd;
    cin >> cmd; 
    cmd = toLower(cmd);
    
    cout << endl;
    if (cmd == "start") {
        startMethod();

    } else if (cmd == "status") {
        statusMethod();

    } else if (cmd == "run") {
        
        cout << "Please, insert number of interations:" << endl;
        cin >> Y;
        runMethod();
        
    } else if (cmd == "stop") {
        stopMethod();
             
    } else if (cmd == "quit") {
        quitMethod();
       
    } else if (cmd == "help") {
        helpMethod();
       
    } else {
        cout << endl << "Wrong command" << endl;
    }
    
}

void test() {
    
    N = 4;
    M = 4;
    int test_num = 15;
    
    for (int t = 0; t < test_num; ++t) {
        
        int i = rand() % N;
        int j = rand() % M;
        int num = getCellNum(i, j);
        int i_, j_;
        getCellCoords(num, i_, j_);
        cout << i << " : " << j << endl << num << endl << i_ << " : " << j << endl << endl;        
    }
    
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    srand(time(NULL));
    status = stopped;    
    
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    if (world_rank == 0) {
        MPI_Comm_split(MPI_COMM_WORLD, 0, world_rank, &master_comm);        
        
        int loc_rank;
        MPI_Comm_rank(master_comm, &loc_rank);
        
        int loc_size;
        MPI_Comm_size(master_comm, &loc_size);
        
        cout << "MASTER: Rank/size: " << loc_rank << "/" << loc_size << endl;
        
        startMethod();
        
    } else {
        MPI_Comm_split(MPI_COMM_WORLD, 1, world_rank, &work_comm);
        
        int loc_rank;
        MPI_Comm_rank(work_comm, &loc_rank);
        
        int loc_size;
        MPI_Comm_size(work_comm, &loc_size);
        
        cout << "WORKER: Rank/size: " << loc_rank << "/" << loc_size << endl;
    }
    
    
//    test();
/*    
    cout << "HELLO!" << endl;
    startMethod();
    helpMethod();
    
    while (status != on_quit) {
        cout << "\n\n";
        readCommand();
    }
    
    cout << "Good bye.." << endl;
  */  
    MPI_Finalize();
    return 0;
}

