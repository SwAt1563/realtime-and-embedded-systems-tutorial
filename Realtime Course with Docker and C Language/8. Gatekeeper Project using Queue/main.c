
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"

void create_all_message_queues();
void create_Q(int key);

void clean_all_message_queues();
void clean_Q(int key);

int main(int argc, char** argv){


    create_all_message_queues();

    pid_t gate_keeper = fork();
    if(gate_keeper == 0){
        execlp("./gate_keeper", "gate_keeper", NULL);
    }


    pid_t person1 = fork();
    if(person1 == 0){
        execlp("./person", "person", "1", "Qutaiba", NULL);
    }

    pid_t person2 = fork();
    if(person2 == 0){
        execlp("./person", "person", "1", "Omar", NULL);
    }

    pid_t person3 = fork();
    if(person3 == 0){
        execlp("./person", "person", "2", "Zenia", NULL);
    }

    pid_t person4 = fork();
    if(person4 == 0){
        execlp("./person", "person", "2", "Marwa", NULL);
    }

    pid_t person5 = fork();
    if(person5 == 0){
        execlp("./person", "person", "1", "Samer", NULL);
    }


    sleep(50);
    clean_all_message_queues();

    return 0;
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