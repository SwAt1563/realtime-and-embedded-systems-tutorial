
#ifndef PERSON_HEADER
#define PERSON_HEADER

// Person Gender Type
enum Genders{MALE = 1, FEMALE = 2};

// The Person Request of OIM
// The tickets are labeled Bx for birth certificate requests,
// Tx for travel document requests, Rx for family reunion requests and Ix for ID-related requests.
// The letter x is a serial number
enum Requests{B = 1, T = 2, R= 3, I = 4};

// Person leaving of stay depend on his patience
enum PatienceDegree {LOW, MEDIUM, HIGH, VERY_HIGH};

// The person position
enum Stage{OUTSIDE, MIDDLE, INSIDE};

// The person end status
enum END{CREATION, UNSERVED, UNHAPPY, SATISFIED};

// For change the size of the queue
enum STEP{INCREMENT, DECREMENT};



struct person {
    pid_t id;
    enum Genders gender;
    enum Requests request;
    enum PatienceDegree patience_degree;
    time_t arrival_time_in_hours;
    time_t arrival_time_in_minutes;
    enum Stage stage;

};

/* End Thresholds */
#define CREATION_THRESHOLD 200 // The max number of peoples that we can generate
#define UNSERVED_THRESHOLD 120
#define UNHAPPY_THRESHOLD 20
#define SATISFIED_THRESHOLD 50
/* End Thresholds */

#define SATISFIED_RATIO 85 // 85% SATISFIED AND 15% UNHAPPY IN THE END RESULT



#endif


#ifndef PERSON_FUNCTIONS
#define PERSON_FUNCTIONS

#include "shared_memories.h"


void increment_ends(enum END status){



    // For get the SEM
    int sem_id = shmget(SEM_KEY, sizeof(struct shared_end_memory), IPC_CREAT | 0777);
    if (sem_id == -1){
        perror("SEM Get Error\n");
        exit(-1);
    }


    // For access the SEM
    SEM sem_mem =  shmat(sem_id, NULL, 0);


    if (sem_mem == (void *) -1) {
        perror("SEM Attached Error\n");
        exit(-1);
    }

    // For open the SEM Semaphore
    sem_t* sem_sem = sem_open(SEM_SEM, O_CREAT, 0777, 0);
    if(sem_sem == SEM_FAILED){
        perror("SEM Semaphore Open Error\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(sem_sem) < 0){
        perror("SEM Semaphore Wait Error\n");
        exit(-1);
    }

    /* Mutex Code */
    switch (status) {
        case CREATION:sem_mem->number_of_people += 1;
            break;
        case UNSERVED:sem_mem->unserved += 1;
            break;
        case UNHAPPY:sem_mem->unhappy += 1;
            break;
        case SATISFIED:sem_mem->satisfied += 1;
            break;
    }
    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(sem_sem) < 0){
        perror("SEM Semaphore Release Error\n");
        exit(-1);
    }


    // Close the SEM Semaphore
    sem_close(sem_sem);

    // Detach the SEM
    if (shmdt(sem_mem) == -1) {
        perror("SEM Detached Error\n");
        exit(-1);
    }


}


unsigned int read_ends(enum END status){

    unsigned int end = 0;

    // For get the SEM
    int sem_id = shmget(SEM_KEY, sizeof(struct shared_end_memory), IPC_CREAT | 0777);
    if (sem_id == -1){
        perror("SEM Get Error\n");
        exit(-1);
    }


    // For access the SEM
    SEM sem_mem =  shmat(sem_id, NULL, 0);


    if (sem_mem == (void *) -1) {
        perror("SEM Attached Error\n");
        exit(-1);
    }

    // For open the SEM Semaphore
    sem_t* sem_sem = sem_open(SEM_SEM, O_CREAT, 0777, 0);
    if(sem_sem == SEM_FAILED){
        perror("SEM Semaphore Open Error\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(sem_sem) < 0){
        perror("SEM Semaphore Wait Error\n");
        exit(-1);
    }


    /* Mutex Code */
    switch (status) {
        case 0:end = sem_mem->number_of_people;
            break;
        case 1:end = sem_mem->unserved ;
            break;
        case 2:end = sem_mem->unhappy ;
            break;
        case 3:end = sem_mem->satisfied ;
            break;
    }
    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(sem_sem) < 0){
        perror("SEM Semaphore Release Error\n");
        exit(-1);
    }


    // Close the SEM Semaphore
    sem_close(sem_sem);

    // Detach the SEM
    if (shmdt(sem_mem) == -1) {
        perror("SEM Detached Error\n");
        exit(-1);
    }

    return end;


}


void inc_dec_queue_size(enum Stage stage, enum Genders gender, enum STEP step){

    int value;
    switch (step) {
        case INCREMENT: value = 1;
            break;
        case DECREMENT: value = -1;
            break;
    }


    // For get the SQSM
    int sqsm_id = shmget(SQSM_KEY, sizeof(struct shared_queue_size_memory), IPC_CREAT | 0777);
    if (sqsm_id == -1){
        perror("SQSM Get Error\n");
        exit(-1);
    }


    // For access the SQSM
    SQSM sqsm_mem =  shmat(sqsm_id, NULL, 0);


    if (sqsm_mem == (void *) -1) {
        perror("SQSM Attached Error\n");
        exit(-1);
    }

    // For open the SQSM Semaphore
    sem_t* sqsm_sem = sem_open(SQSM_SEM, O_CREAT, 0777, 0);
    if(sqsm_sem == SEM_FAILED){
        perror("SQSM Semaphore Open Error\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(sqsm_sem) < 0){
        perror("SQSM Semaphore Wait Error\n");
        exit(-1);
    }


    /* Mutex Code */

    switch (stage) {

        case OUTSIDE:
            switch (gender) {

                case MALE:sqsm_mem->outside_male_queue_size += value;

                    break;
                case FEMALE:sqsm_mem->outside_female_queue_size += value;
                    break;
            }
            break;
        case MIDDLE:
            switch (gender) {
                case MALE:sqsm_mem->middle_male_queue_size += value;

                    break;
                case FEMALE:sqsm_mem->middle_female_queue_size += value;

                    break;
            }
            break;
        case INSIDE:sqsm_mem->inside_queue_size += value;
            break;
    }

    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(sqsm_sem) < 0){
        perror("SQSM Semaphore Release Error\n");
        exit(-1);
    }


    // Close the SQSM Semaphore
    sem_close(sqsm_sem);

    // Detach the SQSM
    if (shmdt(sqsm_mem) == -1) {
        perror("SQSM Detached Error\n");
        exit(-1);
    }



}

unsigned int read_queue_size(enum Stage stage, enum Genders gender){

    unsigned int size = 0;

    // For get the SQSM
    int sqsm_id = shmget(SQSM_KEY, sizeof(struct shared_queue_size_memory), IPC_CREAT | 0777);
    if (sqsm_id == -1){
        perror("SQSM Get Error\n");
        exit(-1);
    }


    // For access the SQSM
    SQSM sqsm_mem =  shmat(sqsm_id, NULL, 0);


    if (sqsm_mem == (void *) -1) {
        perror("SQSM Attached Error\n");
        exit(-1);
    }

    // For open the SQSM Semaphore
    sem_t* sqsm_sem = sem_open(SQSM_SEM, O_CREAT, 0777, 0);
    if(sqsm_sem == SEM_FAILED){
        perror("SQSM Semaphore Open Error\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(sqsm_sem) < 0){
        perror("SQSM Semaphore Wait Error\n");
        exit(-1);
    }


    /* Mutex Code */

    switch (stage) {
        case OUTSIDE:
            switch (gender) {
                case MALE:size = sqsm_mem->outside_male_queue_size ;
                    break;
                case FEMALE:size = sqsm_mem->outside_female_queue_size ;
                    break;
            }
            break;
        case MIDDLE:
            switch (gender) {
                case MALE:size = sqsm_mem->middle_male_queue_size ;
                    break;
                case FEMALE:size = sqsm_mem->middle_female_queue_size ;
                    break;
            }
            break;
        case INSIDE:size = sqsm_mem->inside_queue_size ;
            break;
    }

    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(sqsm_sem) < 0){
        perror("SQSM Semaphore Release Error\n");
        exit(-1);
    }


    // Close the SQSM Semaphore
    sem_close(sqsm_sem);

    // Detach the SQSM
    if (shmdt(sqsm_mem) == -1) {
        perror("SQSM Detached Error\n");
        exit(-1);
    }

    return size;

}


void inc_dec_tellers_numbers(enum Requests request, enum STEP step){

    int value;
    switch (step) {
        case INCREMENT: value = 1;
            break;
        case DECREMENT: value = -1;
            break;
    }


    // For get the STPM
    int stpm_id = shmget(STPM_KEY, sizeof(struct shared_tellers_processing_memory), IPC_CREAT | 0777);
    if (stpm_id == -1){
        perror("STPM Get Error\n");
        exit(-1);
    }


    // For access the STPM
    STPM stpm_mem =  shmat(stpm_id, NULL, 0);


    if (stpm_mem == (void *) -1) {
        perror("STPM Attached Error\n");
        exit(-1);
    }

    // For open the STPM Semaphore
    sem_t* stpm_sem = sem_open(STPM_SEM, O_CREAT, 0777, 0);
    if(stpm_sem == SEM_FAILED){
        perror("STPM Semaphore Open Error\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(stpm_sem) < 0){
        perror("STPM Semaphore Wait Error\n");
        exit(-1);
    }


    /* Mutex Code */

    switch (request) {
        case T:stpm_mem->t_tellers_numbers += value;break;
        case R:stpm_mem->r_tellers_numbers += value;break;
        case I:stpm_mem->i_tellers_numbers += value;break;
        case B:stpm_mem->b_tellers_numbers += value;break;
    }

    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(stpm_sem) < 0){
        perror("STPM Semaphore Release Error\n");
        exit(-1);
    }


    // Close the STPM Semaphore
    sem_close(stpm_sem);

    // Detach the STPM
    if (shmdt(stpm_mem) == -1) {
        perror("STPM Detached Error\n");
        exit(-1);
    }


}

unsigned int read_tellers_numbers(enum Requests request){

    unsigned int size = 0;


    // For get the STPM
    int stpm_id = shmget(STPM_KEY, sizeof(struct shared_tellers_processing_memory), IPC_CREAT | 0777);
    if (stpm_id == -1){
        perror("STPM Get Error\n");
        exit(-1);
    }


    // For access the STPM
    STPM stpm_mem =  shmat(stpm_id, NULL, 0);


    if (stpm_mem == (void *) -1) {
        perror("STPM Attached Error\n");
        exit(-1);
    }

    // For open the STPM Semaphore
    sem_t* stpm_sem = sem_open(STPM_SEM, O_CREAT, 0777, 0);
    if(stpm_sem == SEM_FAILED){
        perror("STPM Semaphore Open Error\n");
        exit(-1);
    }


    // When return 0 for successful access
    if (sem_wait(stpm_sem) < 0){
        perror("STPM Semaphore Wait Error\n");
        exit(-1);
    }


    /* Mutex Code */

    switch (request) {
        case T:size=stpm_mem->t_tellers_numbers;break;
        case R:size=stpm_mem->r_tellers_numbers;break;
        case I:size=stpm_mem->i_tellers_numbers;break;
        case B:size=stpm_mem->b_tellers_numbers;break;
    }

    /* End Mutex Code */


    // When return -1 then wrong issue happened
    if (sem_post(stpm_sem) < 0){
        perror("STPM Semaphore Release Error\n");
        exit(-1);
    }


    // Close the STPM Semaphore
    sem_close(stpm_sem);

    // Detach the STPM
    if (shmdt(stpm_mem) == -1) {
        perror("STPM Detached Error\n");
        exit(-1);
    }

    return size;

}



#endif

