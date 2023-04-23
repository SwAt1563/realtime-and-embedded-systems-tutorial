
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"
#include "semphores.h"
#include "shared_memories.h"

sem_t mutex;

struct args {
    int x;
    double y;
};

int value = 150;

void employee1(void* args){

    int* emp_id = (int*) args;
    value = 600; // the thread1 change the shared global variable from 150 to 600

    while (1){

        sem_wait(&mutex);

        printf("I'm %d doing the task 1\n", *emp_id);
        sleep(3);

        sem_post(&mutex);

        printf("I %d finished the task 1\n", *emp_id);
        sleep(2);
    }


}

void employee2(void* args){
    sleep(2);
    int* emp_id = (int*) args;

    // the global data shared between threads
    printf("The value is :%d\n", value); // will print 600 because the thread1 changed it

    while (1){
        sem_wait(&mutex);

        printf("I'm %d doing the task 2\n", *emp_id);
        sleep(3);

        sem_post(&mutex);

        printf("I %d finished the task 2\n", *emp_id);
        sleep(2);
    }

}

void args_testing(void* args){
    struct args* data = (struct args*) args;

    printf("the x is: %d\n", data->x);
    printf("the y is: %lf\n", data->y);

}



int main(int argc, char** argv){


    // (mutex, {0:shared between threads, 1: shared between process}, mutex value)
    sem_init(&mutex, 0, 1);


    pthread_t emp1, emp2, args_thread;
    int emp1_id = 10;
    int emp2_id = 20;


    struct args parameters;
    parameters.x = 100;
    parameters.y = 300;

    pthread_create(&emp1, NULL, employee1, (void *)&emp1_id);
    pthread_create(&emp2, NULL, employee2, (void *)&emp2_id);
    pthread_create(&args_thread, NULL, args_testing, (void *)&parameters);


    pthread_join(emp1, NULL);
    pthread_join(emp2, NULL);
    pthread_join(args_thread, NULL);

    sem_destroy(&mutex);

    return 0;
}

