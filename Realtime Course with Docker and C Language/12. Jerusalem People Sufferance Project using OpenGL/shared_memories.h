
#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES


// Shared Time Memory Key
#define STM_KEY 0x7341

struct shared_time_memory{

    time_t current_hours;
    time_t current_minutes;


};

typedef struct shared_time_memory* STM;

// Semaphore for STM
const char *STM_SEM = "stm_mutex196652";



// Shared End Memory Key
#define SEM_KEY 0x6224

struct shared_end_memory{

    unsigned int number_of_people;
    unsigned int unserved;
    unsigned int unhappy;
    unsigned int satisfied;



};

typedef struct shared_end_memory* SEM;

// Semaphore for SEM
const char *SEM_SEM = "sem_mutex51235";




// Shared Queue Size Memory Key
#define SQSM_KEY 0x1616

struct shared_queue_size_memory{

    unsigned int outside_male_queue_size;
    unsigned int outside_female_queue_size;
    unsigned int middle_male_queue_size;
    unsigned int middle_female_queue_size;
    unsigned int inside_queue_size;


};

typedef struct shared_queue_size_memory* SQSM;

// Semaphore for SQSM
const char *SQSM_SEM = "sqsm_mutex11538";





// Shared Tellers Processing Memory Key
#define STPM_KEY 0x1149


struct shared_tellers_processing_memory{

    unsigned int b_tellers_numbers;
    unsigned int t_tellers_numbers;
    unsigned int r_tellers_numbers;
    unsigned int i_tellers_numbers;

};

typedef struct shared_tellers_processing_memory* STPM;

// Semaphore for STPM
const char *STPM_SEM = "stpm_mutex42424";

#endif