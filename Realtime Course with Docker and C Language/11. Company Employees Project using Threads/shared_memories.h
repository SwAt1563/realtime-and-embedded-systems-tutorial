#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES

#include "header.h"
#include "semphores.h"



int get_shared_memory(int key, int memory_struct_size){


    // For get the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Get Error\n");
        exit(-1);
    }

    return sm_id;
}





#endif
