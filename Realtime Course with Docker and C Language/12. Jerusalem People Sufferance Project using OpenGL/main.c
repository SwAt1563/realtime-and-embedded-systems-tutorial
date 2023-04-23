
/* Our Team */
// 1190760 -> Qutaiba Olayyan - The Leader of this project -
// 1190283 -> Ibrahim Duhadi
// 1191636 -> Zakiya Abumurra
/* Our Team */

#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"
#include "person_header.h"
#include "message_queues.h"


void create_all_shared_memories();
void create_STM();
void create_SEM();
void create_SQSM();
void create_STPM();

void clean_all_shared_memories();
void clean_STM();
void clean_SEM();
void clean_SQSM();
void clean_STPM();


void create_all_semaphores();
void create_STM_semaphore();
void create_SEM_semaphore();
void create_SQSM_semaphore();
void create_STPM_semaphore();

void clean_all_semaphores();
void clean_STM_semaphore();
void clean_SEM_semaphore();
void clean_SQSM_semaphore();
void clean_STPM_semaphore();


void create_all_message_queues();
void create_OQ();
void create_MQ();
void create_PIQ();

void clean_all_message_queues();
void clean_OQ();
void clean_MQ();
void clean_PIQ();






int main( int argc, char** argv )
{


    create_all_shared_memories();
    create_all_semaphores();
    create_all_message_queues();

    pid_t timer, generator, male_door, female_door, security, drawer;
    int tellers[4]; // 4 TYPES OF REQUESTS - Save the number of tellers on each type -
    pid_t tellers_pids[12]; // For save the pid of the tellers there is maximum 12 tellers
    int tellers_number = 0; // The number of generation tellers


    srand(time(NULL));   // Initialization, should only be called once.
    int r;
    // Each type has random number (1,4) windows
    for(int i = 0; i < 4 ; i++){
        r = 1 + (rand() % 4);      // Returns a pseudo-random integer between 1 and 4.
        tellers[i] = r;
    }

    /* Create the timer */
    if((timer = fork()) == -1){
        perror("The timer fork error\n");
        exit(-1);
    }

    // To let the timer leave the main code and go to timer.c
    if(!timer){
        execlp("./timer", "timer", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the timer */

    /* Create the drawer */
    if((drawer = fork()) == -1){
        perror("The drawer fork error\n");
        exit(-1);
    }

    // To let the drawer leave the main code and go to drawer.c
    if(!drawer){
        execlp("./drawer", "drawer", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the drawer */

    /* Create the generator */
    if((generator = fork()) == -1){
        perror("The generator fork error\n");
        exit(-1);
    }

    // To let the generator leave the main code and go to generator.c
    if(!generator){
        execlp("./generator", "args", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the generator */

    /* Create the Male Door */
    if((male_door = fork()) == -1){
        perror("The male_door fork error\n");
        exit(-1);
    }

    // To let the male_door leave the main code and go to door.c
    if(!male_door){
        execlp("./door", "male", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the Male Door */


    /* Create the Female Door */
    if((female_door = fork()) == -1){
        perror("The female_door fork error\n");
        exit(-1);
    }

    // To let the female_door leave the main code and go to door.c
    if(!female_door){
        execlp("./door", "female", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the Female Door */


    /* Create the Security */
    if((security = fork()) == -1){
        perror("The security fork error\n");
        exit(-1);
    }

    // To let the security leave the main code and go to security.c
    if(!security){
        execlp("./security", "security", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the Security */

    /* Create the Tellers */
    char types[] = {'B', 'T', 'R', 'I'};
    for (int i = 0; i < 4; ++i) {
        char type[2];
        /* Create the windows */
        for (int j = 0; j < tellers[i]; ++j) {
            pid_t teller;

            if((teller = fork()) == -1){
                perror("The teller fork error\n");
                exit(-1);
            }

            // To let the teller leave the main code and go to teller.c
            if(!teller){
                sprintf(type,"%c", types[i]); // convert to string
                execlp("./teller", type, (char *) NULL);

                // If the program not have enough memory then will raise error
                perror("exec Failure\n");
                exit(-1);
            }

            tellers_pids[tellers_number++] = teller;

        }


    }
    /* End Create the Tellers */


    /* Check the threshold for end the program */
    while (1){
        unsigned int creation = read_ends(CREATION);
        unsigned int unserved = read_ends(UNSERVED);
        unsigned int unhappy = read_ends(UNHAPPY);
        unsigned int satisfied = read_ends(SATISFIED);

        if ( unserved >= UNSERVED_THRESHOLD || unhappy >= UNHAPPY_THRESHOLD || satisfied >= SATISFIED_THRESHOLD)
            break; // END THE PROGRAM

        printf("Queues Sizes: %d,%d,%d,%d,%d\n", read_queue_size(OUTSIDE, MALE), read_queue_size(OUTSIDE, FEMALE),read_queue_size(MIDDLE, MALE),read_queue_size(MIDDLE, FEMALE),read_queue_size(INSIDE, MALE));
        printf("Ends: %d,%d,%d,%d\n", creation, unserved, unhappy, satisfied);
        printf("Tellers: %d,%d,%d,%d\n", read_tellers_numbers(T), read_tellers_numbers(R), read_tellers_numbers(I),
               read_tellers_numbers(B));
        printf("************************************\n");
        sleep(6 * FAKE_MINUTE);
    }



    printf("*****The program end*****\n");
    /* See the last result */
    sleep(FAKE_HOUR);

    /* Stop the Drawer */
    kill(drawer, 9);
    sleep(FAKE_HOUR / 2); // sleep 5 real seconds
    /* Stop the Generator */
    kill(generator, SIGINT);
    sleep(FAKE_HOUR); // sleep 10 real seconds
    /* Stop the Timer */
    kill(timer, 9);
    /* Stop the Male Door */
    kill(male_door, 9);
    /* Stop the Female Door */
    kill(female_door, 9);
    /* Stop the Security */
    kill(security, 9);
    /* Stop the Tellers */
    for (int i = 0; i < tellers_number; ++i) {
        kill(tellers_pids[i], 9);
    }


    /* Clean everything */
    clean_all_shared_memories();
    clean_all_semaphores();
    clean_all_message_queues();

    return(0);
}

void create_all_shared_memories(){
    create_STM();
    create_SEM();
    create_SQSM();
    create_STPM();
}
void create_STM(){
    // For create the STM
    int stm_id = shmget(STM_KEY, sizeof(struct shared_time_memory), IPC_CREAT | 0777);
    if (stm_id == -1){
        perror("STM Creation Error\n");
        exit(-1);
    }

    // For access the STM
    STM stm_mem =  shmat(stm_id, NULL, 0);


    if (stm_mem == (void *) -1) {
        perror("STM Attached Error\n");
        exit(-1);
    }

    // For initialize the first value of the time
    stm_mem->current_hours = 5; // Start from 5AM
    stm_mem->current_minutes = 0;


    if (shmdt(stm_mem) == -1) {
        perror("STM Detached Error\n");
        exit(-1);
    }

}
void create_SEM(){
    // For create the SEM
    int sem_id = shmget(SEM_KEY, sizeof(struct shared_end_memory), IPC_CREAT | 0777);
    if (sem_id == -1){
        perror("SEM Creation Error\n");
        exit(-1);
    }


    // For access the SEM
    SEM sem_mem =  shmat(sem_id, NULL, 0);


    if (sem_mem == (void *) -1) {
        perror("SEM Attached Error\n");
        exit(-1);
    }

    // For initialize the first value
    sem_mem->unserved = 0;
    sem_mem->unhappy = 0;
    sem_mem->satisfied = 0;
    sem_mem->number_of_people = 0;


    if (shmdt(sem_mem) == -1) {
        perror("SEM Detached Error\n");
        exit(-1);
    }

}
void create_SQSM(){
    // For create the SQSM
    int sqsm_id = shmget(SQSM_KEY, sizeof(struct shared_queue_size_memory), IPC_CREAT | 0777);
    if (sqsm_id == -1){
        perror("SQSM Creation Error\n");
        exit(-1);
    }

    // For access the SQSM
    SQSM sqsm_mem =  shmat(sqsm_id, NULL, 0);


    if (sqsm_mem == (void *) -1) {
        perror("SQSM Attached Error\n");
        exit(-1);
    }

    // For initialize the first value of the sizes
    sqsm_mem->outside_male_queue_size = 0;
    sqsm_mem->outside_female_queue_size = 0;
    sqsm_mem->middle_male_queue_size = 0;
    sqsm_mem->middle_female_queue_size = 0;
    sqsm_mem->outside_male_queue_size = 0;


    if (shmdt(sqsm_mem) == -1) {
        perror("SQSM Detached Error\n");
        exit(-1);
    }
}
void create_STPM(){
    // For create the STPM
    int stpm_id = shmget(STPM_KEY, sizeof(struct shared_tellers_processing_memory), IPC_CREAT | 0777);
    if (stpm_id == -1){
        perror("STPM Creation Error\n");
        exit(-1);
    }

    // For access the STPM
    STPM stpm_mem =  shmat(stpm_id, NULL, 0);


    if (stpm_mem == (void *) -1) {
        perror("STPM Attached Error\n");
        exit(-1);
    }

    // For initialize the first value of the sizes
    stpm_mem->b_tellers_numbers = 0;
    stpm_mem->t_tellers_numbers = 0;
    stpm_mem->i_tellers_numbers = 0;
    stpm_mem->r_tellers_numbers = 0;


    if (shmdt(stpm_mem) == -1) {
        perror("STPM Detached Error\n");
        exit(-1);
    }
}

void clean_all_shared_memories(){
    clean_STM();
    clean_SEM();
    clean_SQSM();
    clean_STPM();
}
void clean_STM(){

    // For get the STM
    int stm_id = shmget(STM_KEY, sizeof(struct shared_time_memory), IPC_CREAT | 0777);
    if (stm_id == -1){
        perror("STM Get Error\n");
        exit(-1);
    }

    // clean the memory
    if(shmctl(stm_id,IPC_RMID, 0) == -1){
        perror("STM Remove Error\n");
        exit(-1);
    }
}
void clean_SEM(){

    // For get the SEM
    int sem_id = shmget(SEM_KEY, sizeof(struct shared_end_memory), IPC_CREAT | 0777);
    if (sem_id == -1){
        perror("SEM Get Error\n");
        exit(-1);
    }

    // clean the memory
    if(shmctl(sem_id,IPC_RMID, 0) == -1){
        perror("SEM Remove Error\n");
        exit(-1);
    }
}
void clean_SQSM(){

    // For get the SQSM
    int sqsm_id = shmget(SQSM_KEY, sizeof(struct shared_queue_size_memory), IPC_CREAT | 0777);
    if (sqsm_id == -1){
        perror("SQSM Get Error\n");
        exit(-1);
    }

    // clean the memory
    if(shmctl(sqsm_id,IPC_RMID, 0) == -1){
        perror("SQSM Remove Error\n");
        exit(-1);
    }
}
void clean_STPM(){

    // For get the STPM
    int stpm_id = shmget(STPM_KEY, sizeof(struct shared_tellers_processing_memory), IPC_CREAT | 0777);
    if (stpm_id == -1){
        perror("STPM Get Error\n");
        exit(-1);
    }

    // clean the memory
    if(shmctl(stpm_id,IPC_RMID, 0) == -1){
        perror("STPM Remove Error\n");
        exit(-1);
    }
}

void create_all_semaphores(){
    create_STM_semaphore();
    create_SEM_semaphore();
    create_SQSM_semaphore();
    create_STPM_semaphore();
}
void create_STM_semaphore(){
    sem_t *stm_sem = sem_open(STM_SEM, O_CREAT, 0777, 0);
    if(stm_sem == SEM_FAILED){
        perror("STM Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(stm_sem) < 0){
        perror("STM Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(stm_sem);
}
void create_SEM_semaphore(){
    sem_t *sem_sem = sem_open(SEM_SEM, O_CREAT, 0777, 0);
    if(sem_sem == SEM_FAILED){
        perror("SEM Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(sem_sem) < 0){
        perror("SEM Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(sem_sem);

}
void create_SQSM_semaphore(){
    sem_t *sqsm_sem = sem_open(SQSM_SEM, O_CREAT, 0777, 0);
    if(sqsm_sem == SEM_FAILED){
        perror("SQSM Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(sqsm_sem) < 0){
        perror("SQSM Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(sqsm_sem);
}
void create_STPM_semaphore(){
    sem_t *stpm_sem = sem_open(STPM_SEM, O_CREAT, 0777, 0);
    if(stpm_sem == SEM_FAILED){
        perror("STPM Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(stpm_sem) < 0){
        perror("STPM Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(stpm_sem);
}

void clean_all_semaphores(){
    clean_STM_semaphore();
    clean_SEM_semaphore();
    clean_SQSM_semaphore();
    clean_STPM_semaphore();
}
void clean_STM_semaphore(){
    if(sem_unlink(STM_SEM) < 0){
        perror("STM Unlink Error\n");
        exit(-1);
    }
}
void clean_SEM_semaphore(){
    if(sem_unlink(SEM_SEM) < 0){
        perror("SEM Unlink Error\n");
        exit(-1);
    }
}
void clean_SQSM_semaphore(){
    if(sem_unlink(SQSM_SEM) < 0){
        perror("SQSM Unlink Error\n");
        exit(-1);
    }
}
void clean_STPM_semaphore(){
    if(sem_unlink(STPM_SEM) < 0){
        perror("STPM Unlink Error\n");
        exit(-1);
    }
}

void create_all_message_queues(){
    create_OQ();
    create_MQ();
    create_PIQ();
}
void create_OQ(){

    if ( msgget(OQ_KEY, IPC_CREAT | 0777) == -1 ) {
        perror("OQ Create Error\n");
        exit(-1);
    }
}
void create_MQ(){
    if ( msgget(MQ_KEY, IPC_CREAT | 0777) == -1 ) {
        perror("MQ Create Error\n");
        exit(-1);
    }
}
void create_PIQ(){
    if ( msgget(PIQ_KEY, IPC_CREAT | 0777) == -1 ) {
        perror("PIQ Create Error\n");
        exit(-1);
    }
}


void clean_all_message_queues(){
    clean_OQ();
    clean_MQ();
    clean_PIQ();
}
void clean_OQ(){
    int queue;
    if ( (queue = msgget(OQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("OQ Get Error\n");
        exit(-1);
    }
    msgctl(queue, IPC_RMID, 0);/* remove */
}
void clean_MQ(){
    int queue;
    if ( (queue = msgget(MQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("MQ Get Error\n");
        exit(-1);
    }
    msgctl(queue, IPC_RMID, 0);/* remove */
}
void clean_PIQ(){
    int queue;
    if ( (queue = msgget(PIQ_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("PIQ Get Error\n");
        exit(-1);
    }
    msgctl(queue, IPC_RMID, 0);/* remove */

}