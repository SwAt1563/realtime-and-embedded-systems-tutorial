
#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"
#include "person_header.h"
#include "message_queues.h"



struct person info;
void fill_data(enum Genders);
void send_info(int);
time_t get_person_patience_threshold();
void change_position(int);
void check_position_then_leave();
int check_patience_end();

int main( int argc, char** argv ) {

    if (argc < 1 ){
        perror("No enough data for person\n");
        exit(-1);
    }
    enum Genders gender = (enum Genders) atoi(argv[0]);
    fill_data(gender);


    /* For put the process information in the PIQ */
    if ( sigset(SIGINT, send_info ) == SIG_ERR)
    {
        perror("Sigset can not set SIGINT\n");
        exit(SIGINT);
    }
    /* For change the position to the next one */
    if ( sigset(SIGILL, change_position ) == SIG_ERR)
    {
        perror("Sigset can not set SIGILL\n");
        exit(SIGILL);
    }



    while (1){

        if (check_patience_end()){

            /* increment number of unserved people */
            increment_ends(UNSERVED);
            /* Decrement the size of the current queue of this person */
            check_position_then_leave();
            break;
        }

        sleep(9 * FAKE_MINUTE);
    }



    return 0;

}

void fill_data(enum Genders gender){


    srand(time(NULL));   // Initialization, should only be called once.
    int r = rand() % 2;      // Returns a pseudo-random integer between 0 and 1.


    // Put the current time as id for the new person
    info.id = getpid();
    // The gender for each person that we picked randomly
    info.gender = gender;
    // The first stage for each person is outside the OIM
    info.stage = OUTSIDE;
    // The arrival time for the person is now
    time_t* current_time = read_current_time();
    info.arrival_time_in_hours = *current_time;
    info.arrival_time_in_minutes = *(current_time+1);

    // The Patience for the person
    // how much time he/she can wait in the OIM
    r = rand() % 4;
    if(r == 0){
        info.patience_degree = LOW;
    }else if(r == 1){
        info.patience_degree = MEDIUM;
    }else if(r == 2){
        info.patience_degree = HIGH;
    }else {
        info.patience_degree = VERY_HIGH;
    }

    // The Request the person want to do in the OIM
    r = rand() % 4;
    if(r == 0){
        info.request = T;
    }else if(r == 1){
        info.request = R;
    }else if(r == 2){
        info.request = I;
    }else {
        info.request = B;
    }


}

void send_info(int sig){


    int PIQ;

    if ( (PIQ = msgget(PIQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("PIQ Get Error\n");
        exit(-1);
    }

    PersonInfoQueue queue;


    queue.request_type = info.request;
    queue.info = info;

    if (msgsnd(PIQ, &queue, sizeof(queue), 0) == -1) {
        perror("PIQ Send Error\n");
        exit(-1);
    }

}


time_t get_person_patience_threshold(){
    time_t threshold = 0;


    switch (info.patience_degree) {
        case LOW: threshold = LOW_DEGREE ;
            break;
        case MEDIUM: threshold = MEDIUM_DEGREE;
            break;
        case HIGH: threshold = HIGH_DEGREE;
            break;
        case VERY_HIGH: threshold = VERY_HIGH_DEGREE;
        break;

    }


    return threshold;
}

void change_position(int sig){

    switch (info.stage) {
        case OUTSIDE:info.stage = MIDDLE;
            inc_dec_queue_size(OUTSIDE, info.gender, DECREMENT);
            inc_dec_queue_size(MIDDLE, info.gender, INCREMENT);
            break;
        case MIDDLE:info.stage = INSIDE;
            inc_dec_queue_size(MIDDLE, info.gender, DECREMENT);
            inc_dec_queue_size(INSIDE, info.gender, INCREMENT);
            break;
        default:
            // The person finished the request
            inc_dec_queue_size(INSIDE, info.gender, DECREMENT);
            exit(0); // End the process

    }

}


void check_position_then_leave(){

    switch (info.stage) {
        case OUTSIDE:inc_dec_queue_size(OUTSIDE, info.gender, DECREMENT);
            break;
        case MIDDLE:inc_dec_queue_size(MIDDLE, info.gender, DECREMENT);
            break;
        case INSIDE:inc_dec_queue_size(INSIDE, info.gender, DECREMENT);
            break;
    }
}



int check_patience_end(){
    time_t *current_time = read_current_time();
    time_t current_hour = *current_time;
    time_t current_minute = *(current_time + 1);

    time_t person_arrival_time_in_hours = info.arrival_time_in_hours;
    time_t person_arrival_time_in_minutes = info.arrival_time_in_minutes;

    time_t threshold = get_person_patience_threshold() * 60; // threshold_in_fake_minutes

    time_t diff_hours = current_hour - person_arrival_time_in_hours;
    time_t diff_minutes = current_minute - person_arrival_time_in_minutes;

    time_t summation = (diff_hours * 60) + diff_minutes;

    // compare by fake minutes
    if (summation >= threshold){
        return 1;
    }
    return 0;

}