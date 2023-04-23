
#ifndef MESSAGE_QUEUES
#define MESSAGE_QUEUES

#include "person_header.h"

 // if we receive the message with (0) value then we don't care about gender
 // we care about order
 // if you want Male then value (1) and (2) for Female
 struct queue{
    long gender; // to bring the message from the queue depend on its type
    pid_t person;
};

typedef struct queue OutsideQueue;
typedef struct queue MiddleQueue;

// Outside Queue Key
#define OQ_KEY 0x1423
// Middle Queue Key
#define MQ_KEY 0x1853

/* Middle Queue Maximum Sizes */
#define MIDDLE_QUEUE_MALE_MAX_SIZE 20
#define MIDDLE_QUEUE_FEMALE_MAX_SIZE 12
/* Middle Queue Maximum Sizes */


/* Inside Queue */
struct person_info{
    long request_type; // to bring the message from the queue depend on its type
    struct person info;
};

typedef struct person_info PersonInfoQueue;

// Person Info Queue Key
#define PIQ_KEY 0x1444



#endif



#ifndef MESSAGE_QUEUES_FUNCTIONS
#define MESSAGE_QUEUES_FUNCTIONS

#include "header.h"
int check_queue_empty(int queue, long queue_type){

    if(msgrcv(queue, NULL, 0, queue_type, IPC_NOWAIT) == -1){
        if(errno == E2BIG)
            return 0; // There is data

    }
    return 1; // Empty queue
}
#endif