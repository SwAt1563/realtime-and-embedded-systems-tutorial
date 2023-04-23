

#include "header.h"



//sem_unlink() : Ends connection to an open semaphore & causes the semaphore to be removed when the last process closes it
// ( like sem_destroy())
void parent(const char *mutex){

    sleep(1);
    sem_t *sem = sem_open(mutex, 0);
    if(sem == SEM_FAILED){
        perror("open error\n");
        exit(-1);
    }
    printf("i'm the parent waiting my child to finish\n");
    if (sem_wait(sem) < 0){
        perror("wait error\n");
        exit(-1);
    }
    printf("i'm the parent my child finished\n");


    printf("i'm the parent the sem closed\n");

    if (sem_post(sem) < 0){
        perror("release error\n");
        exit(-1);
    }


    sem_close(sem);


    if(sem_unlink(mutex) < 0){
        perror("unlink error\n");
        exit(-1);
    }
    printf("i'm the parent the mutex unlinked -destroyed-\n");


}

void child(const char *mutex)
{
    sem_t *sem = sem_open(mutex, 0);
    if(sem == SEM_FAILED){
        perror("open error\n");
        exit(-1);
    }

    if (sem_wait(sem) < 0){
        perror("wait error\n");
        exit(-1);
    }

    printf("i'm child i will sleep 5s\n");
    sleep(10);

    if (sem_post(sem) < 0){
        perror("release error\n");
        exit(-1);
    }
    printf("i'm child released the sem\n");

    sem_close(sem);

}

// gcc -lpthread -lrt
int main(int argc, char *argv[])
{
    const char *semName = "mutex16223";
    sem_t *sem = sem_open(semName, O_CREAT, 0777, 0);
    if(sem == SEM_FAILED){
        perror("open error\n");
        exit(-1);
    }
     if(sem_post(sem) < 0){
        perror("release error\n");
        exit(-1);
    }
    sem_close(sem);

    pid_t parent_p=getpid(), child_p;



    child_p = fork();


    if(child_p == -1){
        exit(-1);
    } else if(child_p == 0){
        child(semName);
    }else {
        parent(semName);
    }

    return 0;
}

