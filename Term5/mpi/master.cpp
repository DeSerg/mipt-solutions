#include "master.h"


int K, N, M, Y;

Status status;

MPI_Comm work_comm;
MPI_Comm master_comm;
MPI_Comm intercomm;

int work_rank, work_size;
int world_rank, world_size;

int **cur_table;
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
    status = before_run;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    if (world_size < 2) {
        cerr << "Too few processes" << endl;
        exit(0);
    }
    
    int color;
    if (world_rank == 0) {
        color = 0;
    } else {
        color = 1;
    }
    
    MPI_Comm newcomm;
    MPI_Comm world_dupcom;
    
    MPI_Comm_dup(MPI_COMM_WORLD, &world_dupcom);
    MPI_Comm_split(MPI_COMM_WORLD, color, 1, &newcomm);
    
    if (world_rank == 0) {
        MPI_Intercomm_create(newcomm, 0, world_dupcom, 1, 111, &intercomm);
    } else {
        MPI_Intercomm_create(newcomm, 0, world_dupcom, 0, 111, &intercomm);
    }
    
    
    /*
    if (world_rank == 0) {
        int remote_size;
        MPI_Comm_remote_size(intercomm, &remote_size);
        cout << "master: remote group size is " << remote_size << endl;
        int k = 0;
        MPI_Bcast(&k, 1, MPI_INT, MPI_ROOT, intercomm);
    } else {
//        MPI_Barrier(newintercomm);
        
        int k;
        MPI_Bcast(&k, 1, MPI_INT, 0, intercomm);
        cout << "MES GOT!" << endl;
    }
*/
    
    MPI_Group work_group_all;
    MPI_Group work_group;
    MPI_Comm_group(MPI_COMM_WORLD, &work_group_all);
    
    const int n[1] = {0};
    MPI_Group_excl(work_group_all, 1, n, &work_group);
    
    MPI_Comm_create(MPI_COMM_WORLD, work_group, &work_comm);
    
    if (world_rank != 0) {
        MPI_Comm_size(work_comm, &work_size);
        MPI_Comm_rank(work_comm, &work_rank);
    }
    
    if (world_rank == 0) {
        
        cerr << "HELLO!" << endl;
        startMethod();
        helpMethod();
        
        while (status != on_quit) {
            cerr << "\n\n";
            readCommand();
        }
        
        cout << "Good bye.." << endl;
        
    } else {
        work();
    }

    deallocArray(init_table, N);
    MPI_Finalize();
    
    return 0;
}

