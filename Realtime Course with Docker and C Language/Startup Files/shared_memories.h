#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES

#include "header.h"
#include "semaphores.h"


struct shared_memory {
    int value;
};


#define SM_KEY 0x1478

typedef struct shared_memory* SM;



int get_shared_memory(int key, int memory_struct_size){


    // For get the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Get Error\n");
        exit(-1);
    }

    return sm_id;
}

void update_shared_memory(struct shared_memory data){

    int sm_id = get_shared_memory(SM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(SM_SEMAPHORE);


    // For access the SM
    SM sm_mem =  shmat(sm_id, NULL, 0);


    if (sm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm);

    // update data here
    sm_mem->value = data.value;
    

    detach_semaphore(sm);

    if (shmdt(sm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }


    
}

   


struct shared_memory read_shared_memory(){

    struct shared_memory data;

    int sm_id = get_shared_memory(SM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(SM_SEMAPHORE);



    // For access the SM
    SM sm_mem =  shmat(sm_id, NULL, 0);


    if (sm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm);

    // read data here
    data.value = sm_mem->value;
    

    detach_semaphore(sm);

    if (shmdt(sm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }


    return data;
}





#endif
