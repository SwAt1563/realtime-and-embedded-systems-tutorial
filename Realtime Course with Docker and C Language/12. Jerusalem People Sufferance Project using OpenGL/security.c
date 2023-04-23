
#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"
#include "person_header.h"
#include "message_queues.h"




int main( int argc, char** argv ) {


    srand(time(NULL));   // Initialization, should only be called once.
    int r;


    /* For access the Middle */
    int MQ;

    if ( (MQ = msgget(MQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("MQ Get Error\n");
        exit(-1);
    }


    MiddleQueue middle_queue;

    L: while (1){

        r = (enum Genders) (1 + (rand() % 2)); // return 1(Male) or 2(Female)

        switch (r) {
            case MALE:
                /* Here you should check the size of the queue if empty then switch */
                if(read_queue_size(MIDDLE, MALE) < 1){
                    sleep(3 * FAKE_MINUTE);
                    goto L;
                }

                if(msgrcv(MQ, &middle_queue, sizeof(middle_queue), MALE, 0) == -1){
                    perror("Middle Queue received error\n");
                    exit(-1);
                }
                goto IN;
            case FEMALE:
                /* Here you should check the size of the queue if empty then switch */
                if(read_queue_size(MIDDLE, FEMALE) < 1){
                    sleep(3 * FAKE_MINUTE);
                    goto L;
                }

                if(msgrcv(MQ, &middle_queue, sizeof(middle_queue), FEMALE, 0) == -1){
                    perror("Middle Queue received error\n");
                    exit(-1);
                }

                goto IN;
        }



        IN:;


        /* For change the position of the person from middle to inside */
        // if the kill return -1 then the person left the OIM
        if ( kill (middle_queue.person, SIGILL) != -1){

            // For let the person send his information to the inner queue
            kill (middle_queue.person, SIGINT);

        }
        usleep(SECURITY_PROCESSING_TIME * 1000000);


    }
    pause();
    return 0;
}