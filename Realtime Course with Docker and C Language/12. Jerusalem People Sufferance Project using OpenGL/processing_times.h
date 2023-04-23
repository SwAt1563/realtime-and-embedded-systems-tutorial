
#ifndef P_TIMES
#define P_TIMES

/* Time Rendering */

//real_second == 3 fake_minutes
//20 real_seoncds == 1 fake_hour
//1 real_minute == 3 fake_hours

/* End Time Rendering */

// The value related to => sleep(value)
#define FAKE_HOUR 10
#define FAKE_MINUTE 1 / 3.0


#define MORNING_TIME_PERIOD_DURATION 3  // From 5AM To 8AM
#define EVENING_TIME_PERIOD_DURATION 5  // From 8AM To 1PM

// Increment the cloning timer each minute
// The fake time will increase 1 fake minutes each 1 / 3 real second
#define TIME_STEP_INCREMENT FAKE_MINUTE


// Generate person each 3 fake minutes - each real second will generate one person -
#define GENERATING_PERSON_STEP 3 * FAKE_MINUTE

// The amount of time a person can afford
// the patience for the person for leaving or not in fake hours
#define LOW_DEGREE 2
#define MEDIUM_DEGREE 4
#define HIGH_DEGREE 6
#define VERY_HIGH_DEGREE 8


// Middle Queues Times
// How Much Time to receive a person by the door.c
#define MALE_QUEUE_RECEIVING_TIME 2 * FAKE_MINUTE
#define FEMALE_QUEUE_RECEIVING_TIME 3 * FAKE_MINUTE

// Security Processing Time
#define SECURITY_PROCESSING_TIME 4 * FAKE_MINUTE

// Teller Processing Time
#define TELLER_PROCESSING_TIME 24 * FAKE_MINUTE

#endif




#ifndef P_TIMES_FUNCTIONS
#define P_TIMES_FUNCTIONS


#include "shared_memories.h"

time_t* read_current_time(){
    static time_t current_time[2];

    // For get the STM
    int stm_id = shmget(STM_KEY, sizeof(struct shared_time_memory), IPC_CREAT | 0777);
    if (stm_id == -1){
        perror("STM Get Error\n");
        exit(-1);
    }


    // For access the STM
    STM stm_mem =  shmat(stm_id, NULL, 0);


    if (stm_mem == (void *) -1) {
        perror("STM Attached Error\n");
        exit(-1);
    }

    // For open the STM Semaphore
    sem_t* stm_sem = sem_open(STM_SEM, 0);
    if(stm_sem == SEM_FAILED){
        perror("STM Semaphore Open Error, The timer stopped, then stop my process\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(stm_sem) < 0){
        perror("STM Semaphore Wait Error\n");
        exit(-1);
    }

    /* Mutex Code */
    current_time[0] = stm_mem->current_hours;
    current_time[1] = stm_mem->current_minutes;
    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(stm_sem) < 0){
        perror("STM Semaphore Release Error\n");
        exit(-1);
    }

    // Close the STM Semaphore
    sem_close(stm_sem);

    // Detach the STM
    if (shmdt(stm_mem) == -1) {
        perror("STM Detached Error\n");
        exit(-1);
    }

    return current_time;
}

char* time_to_string(){
    static char string_time[6];
    time_t *time = read_current_time();
    sprintf(string_time,"%02ld:%02ld", *time, *(time + 1)); // convert to string
    return &(string_time[0]);
}

#endif
