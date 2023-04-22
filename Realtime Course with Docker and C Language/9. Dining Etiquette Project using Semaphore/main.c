
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"
#include "semphores.h"

/*
 * value = 100
 * P1: value++
 * P2: value--
 * P3: value * 2
 */


void create_all_semaphores();
void create_semaphore(char* key);

void clean_all_semaphores();
void clean_semaphore(char* key);


int main(int argc, char** argv){

    create_all_semaphores();

    pid_t father = fork();

    if ( father == 0){

        sem_t* sem = get_semaphore(EATING_SEMAPHORE);
        attach_semaphore(sem);

        printf("I'm the father and i'm eating now\n");
        sleep(10);
        printf("I'm the father and i finished eating\n");

        detach_semaphore(sem);
        return 0;

    }

    pid_t child = fork();

    if ( child == 0){

        sem_t* sem = get_semaphore(EATING_SEMAPHORE);
        attach_semaphore(sem);

        printf("I'm the child and i'm eating now\n");
        sleep(10);
        printf("I'm the child and i finished eating\n");

        detach_semaphore(sem);

        return 0;
    }


    sleep(50);
    clean_all_semaphores();

    return 0;
}


void create_all_semaphores(){
    create_semaphore(EATING_SEMAPHORE);
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
    clean_semaphore(EATING_SEMAPHORE);
}
void clean_semaphore(char* key){
    if(sem_unlink(key) < 0){
        perror("Semaphore Unlink Error\n");
        exit(-1);
    }
}
