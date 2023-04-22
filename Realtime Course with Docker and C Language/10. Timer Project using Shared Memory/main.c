
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"
#include "semphores.h"
#include "shared_memories.h"



void create_all_shared_memories();
void create_shared_memory(int key, int memory_struct_size);

void clean_all_shared_memories();
void clean_shared_memory(int key, int memory_struct_size);


void create_all_semaphores();
void create_semaphore(char* key);

void clean_all_semaphores();
void clean_semaphore(char* key);


int main(int argc, char** argv){

    create_all_semaphores();
    create_all_shared_memories();


    pid_t timer = fork();

    if (timer == 0){
        execlp("./timer", "timer", NULL);
        perror("EXEC Error\n");
        exit(-1);
    }


    int stop_delay = 10;

    while (stop_delay-- > 0) {

        printf("The current time is:%d\n", read_time());
        sleep(2);


    }

    kill(timer, 9);


    clean_all_semaphores();
    clean_all_shared_memories();
    return 0;
}


void create_all_semaphores(){
    create_semaphore(TIMER_SEMAPHORE);
}
void create_semaphore(char* key){

    sem_t* sem = sem_open(key, O_CREAT, 0777, 0);
    if(sem == SEM_FAILED){
        perror("Semaphore Creation Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(sem) < 0){
        perror("Semaphore Release Error\n");
        exit(-1);
    }

    sem_close(sem);

}

void clean_all_semaphores(){
    clean_semaphore(TIMER_SEMAPHORE);
}
void clean_semaphore(char* key){
    if(sem_unlink(key) < 0){
        perror("Semaphore Unlink Error\n");
        exit(-1);
    }
}




void create_all_shared_memories(){
    create_shared_memory(TSM_KEY, sizeof(struct shared_memory));

}
void create_shared_memory(int key, int memory_struct_size){

    // For create the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    // For access the TSM
    TSM tsm_mem =  shmat(sm_id, NULL, 0);


    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    tsm_mem->timer = 0;


    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }



}

void clean_all_shared_memories(){
    clean_shared_memory(TSM_KEY, sizeof(struct shared_memory));
}
void clean_shared_memory(int key, int memory_struct_size){

    int sm_id = get_shared_memory(key, memory_struct_size);

    // clean the memory
    if(shmctl(sm_id,IPC_RMID, 0) == -1){
        perror("SM Remove Error\n");
        exit(-1);
    }

}
