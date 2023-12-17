
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"
#include "semaphores.h"
#include "shared_memories.h"



void create_all_semaphores();
void create_semaphore(char* key);

void clean_all_semaphores();
void clean_semaphore(char* key);

void create_all_shared_memories();
void create_shared_memory(int key, int memory_struct_size);

void clean_all_shared_memories();
void clean_shared_memory(int key, int memory_struct_size);

void create_all_message_queues();
void create_Q(int key);

void clean_all_message_queues();
void clean_Q(int key);


int main(int argc, char** argv){

    create_all_semaphores();
    create_all_shared_memories();
    create_all_message_queues();


    sleep(5);

    clean_all_shared_memories();
    clean_all_semaphores();
    clean_all_message_queues();
    
    return 0;
}


void create_all_semaphores(){
    create_semaphore(SM_SEMAPHORE);
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
    clean_semaphore(SM_SEMAPHORE);
}
void clean_semaphore(char* key){
    if(sem_unlink(key) < 0){
        perror("Semaphore Unlink Error\n");
        exit(-1);
    }
}




void create_all_shared_memories(){
    create_shared_memory(SM_KEY, sizeof(struct shared_memory));

}
void create_shared_memory(int key, int memory_struct_size){

    // For create the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    // For access the SM
    SM sm_mem =  shmat(sm_id, NULL, 0);


    if (sm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    // Initialization of the memory
    sm_mem->value = 0;


    if (shmdt(sm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }



}

void clean_all_shared_memories(){
    clean_shared_memory(SM_KEY, sizeof(struct shared_memory));
}
void clean_shared_memory(int key, int memory_struct_size){

    int sm_id = get_shared_memory(key, memory_struct_size);

    // clean the memory
    if(shmctl(sm_id,IPC_RMID, 0) == -1){
        perror("SM Remove Error\n");
        exit(-1);
    }

}


void create_all_message_queues(){
    create_Q(MQ_KEY);


}
void create_Q(int key){

    if ( msgget(key, IPC_CREAT | 0777) == -1 ) {
        perror("Queue Creation Error\n");
        exit(-1);
    }

}

void clean_all_message_queues(){
    clean_Q(MQ_KEY);
}
void clean_Q(int key){
    int queue = get_queue(key);
    msgctl(queue, IPC_RMID, 0);/* remove */

}