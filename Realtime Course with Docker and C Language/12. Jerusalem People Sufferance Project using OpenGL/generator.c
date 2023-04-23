#include "header.h"
#include "shared_memories.h"
#include "person_header.h"
#include "processing_times.h"
#include "message_queues.h"


pid_t persons[CREATION_THRESHOLD];
unsigned int number_of_persons = 0;

OutsideQueue create_person(OutsideQueue);
int check_finish();
void clean_persons(int);

int main(int argc, char *argv[ ])
{

    int OQ;

    if ( (OQ = msgget(OQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("OQ Get Error\n");
        exit(-1);
    }

    OutsideQueue outside_queue;

    sigset(SIGINT, clean_persons);




    while (1) {


        outside_queue = create_person(outside_queue);

        /* wait until person set the signals before insert him in the queue */
        sleep(GENERATING_PERSON_STEP);

        if (msgsnd(OQ, &outside_queue, sizeof(outside_queue), 0) == -1) {
            perror("OQ Send Error\n");
            exit(-1);
        }



        persons[number_of_persons++] = outside_queue.person;

        inc_dec_queue_size(OUTSIDE, outside_queue.gender, INCREMENT);


        /* End generation if the number of people more than max number or the time is 1PM */
        if(check_finish()){
            printf("End Generation\n");
            break;
        }




    }

    pause();
    return 0;
}

OutsideQueue create_person(OutsideQueue message){
    /* Pick random gender */
    srand(time(NULL));   // Initialization, should only be called once.
    int r = rand() % 2;      // Returns a pseudo-random integer between 0 and 1.
    if(r == 1){
        message.gender = MALE;
    }else {
        message.gender = FEMALE;
    }
    /* End Pick random gender */




    pid_t process;

    /* Create the person */
    if((process = fork()) == -1){
        perror("The person fork error\n");
        exit(-1);
    }



    // To let the person leave the generator code and go to person.c
    if(!process){


        char process_gender[2];
        sprintf(process_gender,"%ld", message.gender); // convert to string
        execlp("./person", process_gender, (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the person */


    // save the person process id in the queue
    message.person = process;

    /* increment number of creation people */
    increment_ends(CREATION);
    return message;
}

int check_finish(){

    time_t *current_time = read_current_time();
    time_t current_hour = *current_time;
    time_t the_beginning = 5; // 5AM
    time_t generation_period = MORNING_TIME_PERIOD_DURATION + EVENING_TIME_PERIOD_DURATION; // 3 + 5 = 8 FAKE HOURS
    time_t the_end = the_beginning + generation_period; // 13 o'clock

    if( read_ends(CREATION) >= CREATION_THRESHOLD ||  current_hour >= the_end ){
        return 1;
    }
    return 0;
}

void clean_persons(int sig){
    for (int i = 0; i < number_of_persons; ++i) {
        kill(persons[i], 9);
    }
    printf("All persons removed\n");
    exit(0);
}
