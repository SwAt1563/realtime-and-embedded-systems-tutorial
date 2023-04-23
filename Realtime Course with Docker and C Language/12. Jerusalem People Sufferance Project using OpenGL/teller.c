
#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"
#include "person_header.h"
#include "message_queues.h"


enum Requests type;


int main( int argc, char** argv ) {


    if (argc < 1 ){
        perror("No enough data for teller\n");
        exit(-1);
    }


    if(strcmp(argv[0], "B") == 0){
        type = B;
    }else if(strcmp(argv[0], "T") == 0){
        type = T;
    }else if(strcmp(argv[0], "R") == 0){
        type = R;
    }else if(strcmp(argv[0], "I") == 0){
        type = I;
    }




    /* For access the Inside Queue */
    int PIQ;

    if ( (PIQ = msgget(PIQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("PIQ Get Error\n");
        exit(-1);
    }



    srand(time(NULL));   // Initialization, should only be called once.
    int r ;

    PersonInfoQueue inside_queue;
    enum Requests busy_type = type; // The original type for this teller

    LOOP:while (1){
        r = rand() % 100;      // Returns a pseudo-random integer between 0 and 99.
        if(!check_queue_empty(PIQ, busy_type)){



            if(msgrcv(PIQ, &inside_queue, sizeof(inside_queue), busy_type, 0) == -1){
                perror("Inside Queue received error\n");
                exit(-1);
            }




            /* For increase the number of tellers onm this type */
            inc_dec_tellers_numbers(busy_type, INCREMENT);

            sleep(TELLER_PROCESSING_TIME);

            /* For let this teller leave this type of requests */
            inc_dec_tellers_numbers(busy_type, DECREMENT);


            /* The request result randomly */
            r < SATISFIED_RATIO ? increment_ends(SATISFIED) : increment_ends(UNHAPPY);

            kill(inside_queue.info.id, SIGILL); // For end the person and decrement the size of the inside queue



            goto PROCESS;

        }

        /* For change the teller job to another request */
        for (int i = 1; i < 5; ++i) {
            if(!check_queue_empty(PIQ, (enum Requests) i)){
                busy_type = (enum Requests) i;
                sleep(FAKE_MINUTE * 3);
                goto LOOP;
            }

        }



        PROCESS:;
        sleep(FAKE_MINUTE * 3);
        busy_type = type;
    }


    pause();
    return 0;
}
