#include "master.h"


int K, N, M, Y;

Status status;

MPI_Comm work_comm;
MPI_Comm master_comm;
MPI_Comm intercomm;

int work_rank, work_size;
int world_rank, world_size;

Field init_table;

pthread_t background_thread_t;

void *getData(void *data) {
    
    while (true) {
        int data_flag;
        MPI_Iprobe(MPI_ANY_SOURCE, return_data_tag, MPI_COMM_WORLD, &data_flag, MPI_STATUS_IGNORE);
        
        if (data_flag) {
            
            int count = world_size - 1;
            int integral = N / count;
            int last = N / count + N % count;
            
            MPI_Recv(init_table[0], integral * M, MPI_INT, 0, data_tag, intercomm, MPI_STATUS_IGNORE);
            MPI_Recv(init_table[N - last], last * M, MPI_INT, count - 1, data_tag, intercomm, MPI_STATUS_IGNORE);
            
            for (int i = 1; i < count - 1; ++i) {
                MPI_Recv(init_table[i * integral], integral * M, MPI_INT, i, data_tag, intercomm, MPI_STATUS_IGNORE);
            }
            
            status = stopped;
            
            cerr << "Workers are done!" << endl;
            drawTable(init_table);
        }
        
    }
    
    return NULL;
}

void readCommand() {
    
//    cout << "Please, type the command:" << endl;
    
    string cmd;
    cin >> cmd; 
    cmd = toLower(cmd);
    
    cout << endl;
    if (cmd == "start") {
        startMethod();

    } else if (cmd == "status") {
        statusMethod();

    } else if (cmd == "run") {
        
//        cout << "Please, insert number of interations:" << endl;
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
    
    
    /*
    
    int k = 0;
    if (world_rank == 0) {
        for (int i = 1; i < world_size; ++i) {
            MPI_Request req;
            MPI_Isend(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &req);
        }
    } else {
        int p;
        MPI_Recv(&p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cerr << "gotcha " << p << endl;
    }
    
    */
    
    
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
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &newcomm);
    
    if (world_rank == 0) {
        MPI_Intercomm_create(newcomm, 0, world_dupcom, 1, 111, &intercomm);
    } else {
        MPI_Intercomm_create(newcomm, 0, world_dupcom, 0, 111, &intercomm);
    }
    
    MPI_Group work_group_all;
    MPI_Group work_group;
    MPI_Comm_group(MPI_COMM_WORLD, &work_group_all);
    
    int n[1] = {0};
    MPI_Group_excl(work_group_all, 1, n, &work_group);
    
    MPI_Comm_create(MPI_COMM_WORLD, work_group, &work_comm);
    
    if (world_rank != 0) {
        MPI_Comm_size(work_comm, &work_size);
        MPI_Comm_rank(work_comm, &work_rank);        
    }
    /*
    if (world_rank != 0) {
        for (int i = 1; i < world_size; ++i) {
            
            int inter_rank;
            MPI_Comm_rank(intercomm, &inter_rank);
            
            if (world_rank == i) {
                cout << "world_rank: " << world_rank << ", inter_rank: " << inter_rank << endl;
            }
            
            MPI_Barrier(work_comm);
            
        }
        
    }
    */
    
    if (world_rank == 0) {
        
        cerr << "HELLO!" << endl;
        startMethod();
//        helpMethod();
        
        while (status != on_quit) {
            cerr << "\n\n";
            pthread_create(&background_thread_t, 0, &getData, NULL);
            readCommand();
        }
        
        pthread_cancel(background_thread_t);
        cout << "Good bye.." << endl;
        
    } else {
        work();
    }
    
    
    MPI_Finalize();    
    return 0;
}

