
#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"
#include "person_header.h"
#include "message_queues.h"




enum Genders door_genders;
time_t processing_time;
int check_open();


int main( int argc, char** argv ) {

    /* It should send male or female for this door */
    if (argc < 1 ){
        perror("No enough data for door\n");
        exit(-1);
    }

    /* Choose the type of th door and the processing time */
    if(strcmp(argv[0], "male") == 0){
        door_genders = MALE;
        processing_time = MALE_QUEUE_RECEIVING_TIME;
    }else if (strcmp(argv[0], "female") == 0){
        door_genders = FEMALE;
        processing_time = FEMALE_QUEUE_RECEIVING_TIME;
    }


    /* For access the Outside and Middle Queues */
    int OQ, MQ;

    if ( (OQ = msgget(OQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("OQ Get Error\n");
        exit(-1);
    }

    if ( (MQ = msgget(MQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("MQ Get Error\n");
        exit(-1);
    }

    OutsideQueue outside_queue;
    MiddleQueue middle_queue;

    /* Wait until 8:00AM to open the door */
    while (!check_open()){
        sleep(FAKE_HOUR / 2); // each real 5 second check the fake time is 8am or not
    }



    srand(time(NULL));   // Initialization, should only be called once.
    int r;




    while (1){
        r =  (rand() % 100); // 0 - 99


        if(msgrcv(OQ, &outside_queue, sizeof(outside_queue), door_genders, 0) == -1){
            perror("Outside Queue received error\n");
            exit(-1);
        }

        /*  20% let the person not enter the door and put him again in outside queue */
        if (r > 80){
            if (msgsnd(OQ, &outside_queue, sizeof(outside_queue), 0) == -1) {
                perror("OQ Send Error\n");
                exit(-1);
            }
        }

        if(msgrcv(OQ, &outside_queue, sizeof(outside_queue), door_genders, 0) == -1){
            perror("Outside Queue received error\n");
            exit(-1);
        }



        /* If the middle queue is full then wait until space exist */
        switch (outside_queue.gender) {
            case MALE:
                while (read_queue_size(MIDDLE, MALE) >= MIDDLE_QUEUE_MALE_MAX_SIZE){
                    sleep(3 * FAKE_MINUTE);
                }
                goto PROCESS;
            case FEMALE:
                while (read_queue_size(MIDDLE, FEMALE) >= MIDDLE_QUEUE_FEMALE_MAX_SIZE){
                    sleep(3 * FAKE_MINUTE);
                }
                goto PROCESS;
        }

        PROCESS:;

        /* For change the position of the person from outside to middle */
        // if the kill return -1 then the person left the OIM

        if ( kill (outside_queue.person, SIGILL) != -1){


            middle_queue = outside_queue;
            if (msgsnd(MQ, &middle_queue, sizeof(middle_queue), 0) == -1) {
                perror("MQ Send Error\n");
                exit(-1);
            }

        }


       usleep(processing_time * 1000000);

    }


    pause();
    return 0;

}

int check_open(){
    time_t *current_time = read_current_time();
    time_t current_hour = *current_time;
    time_t the_beginning = 5; // 5AM
    time_t closed_duration = MORNING_TIME_PERIOD_DURATION ; // 3 FAKE HOURS
    time_t the_open = the_beginning + closed_duration; // 8am o'clock

    if(current_hour >= the_open){
        return 1;
    }
    return 0;
}