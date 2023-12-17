#ifndef QUEUES
#define QUEUES

#include "header.h"

struct queue{
    long type;
    char msg[100];
};

typedef struct queue MessagesQueue;

#define MQ_KEY 0x1234



/* For access the Queue */
int get_queue(int key){

    int queue;

    if ( (queue = msgget(key, IPC_CREAT | 0777)) == -1 ) {
        perror("Queue Access Error\n");
        exit(-1);
    }

    return queue;
}


int check_queue_empty(int queue, long queue_type){

    if(msgrcv(queue, NULL, 0, queue_type, IPC_NOWAIT) == -1){
        if(errno == E2BIG)
            return 0; // There is data

    }
    return 1; // Empty queue
}


#endif
