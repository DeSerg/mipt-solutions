#include "master.h"


int K, N, M, Y;

Status status;

MPI_Comm work_comm;
MPI_Comm master_comm;

int **cur_table;
int **cur_table_new;
int **init_table;

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

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    srand(time(NULL));
    status = stopped;    
    
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    MPI_Group work_group_all;
    MPI_Group work_group;
    MPI_Comm_group(MPI_COMM_WORLD, &work_group_all);
    
    int n[1] = {0};
    MPI_Group_excl(work_group_all, 1, n, &work_group);
    MPI_Comm_create(MPI_COMM_WORLD, work_group, work_comm);
    
    if (world_rank == 0) {
//        MPI_Comm_split(MPI_COMM_WORLD, 0, world_rank, &master_comm);        
        
//        int loc_rank;
//        MPI_Comm_rank(master_comm, &loc_rank);
        
//        int loc_size;
//        MPI_Comm_size(master_comm, &loc_size);
        
//        cout << "MASTER: Rank/size: " << loc_rank << "/" << loc_size << endl;
        
//        sleep(5);
        for (int i = 1; i < world_size; ++i) {
            MPI_Send(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
//        startMethod();
        
    } else {
//        MPI_Comm_split(MPI_COMM_WORLD, 1, world_rank, &work_comm);
        
//        int loc_rank;
//        MPI_Comm_rank(work_comm, &loc_rank);
        
//        int loc_size;
//        MPI_Comm_size(work_comm, &loc_size);
        
//        cout << "WORKER: Rank/size: " << loc_rank << "/" << loc_size << endl;
        int work_rank;
        MPI_Comm_rank(work_comm, &work_comm);
        
        int work_size;
        MPI_Comm_size(work_comm, &work_size);
        
        cout << "Hello from work_comm, I am " << work_rank << " of " << work_size << endl;
        
        int ready = 0;
        MPI_Recv(&ready, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
              
        cout << "Worker #" << world_rank << ": I AM READY! (" << ready << ")" << endl;
//        work();
        
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

