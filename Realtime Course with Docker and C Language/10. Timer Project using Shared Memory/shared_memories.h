#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES

#include "header.h"
#include "semphores.h"


struct shared_memory {
    time_t timer;
};


#define TSM_KEY 0x1478

typedef struct shared_memory* TSM;



int get_shared_memory(int key, int memory_struct_size){


    // For get the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Get Error\n");
        exit(-1);
    }

    return sm_id;
}


void inc_timer(){

    int sm_id = get_shared_memory(TSM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(TIMER_SEMAPHORE);



    // For access the TSM
    TSM tsm_mem =  shmat(sm_id, NULL, 0);


    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm);

    tsm_mem->timer++;

    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }



}


time_t read_time(){

    time_t the_current_time = 0;

    int sm_id = get_shared_memory(TSM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(TIMER_SEMAPHORE);



    // For access the TSM
    TSM tsm_mem =  shmat(sm_id, NULL, 0);


    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm);

    the_current_time = tsm_mem->timer;

    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }


    return the_current_time;


}





#endif
