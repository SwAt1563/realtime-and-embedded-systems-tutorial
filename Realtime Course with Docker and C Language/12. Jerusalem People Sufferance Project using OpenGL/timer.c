
#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"

time_t* handle_time(time_t hours, time_t minutes);

int main( int argc, char** argv )
{


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

    sem_t *stm_sem;
    time_t *time;

    // Make Semaphore on the STM when change the time
    while (1){
        stm_sem = sem_open(STM_SEM, 0);
        if(stm_sem == SEM_FAILED){
            perror("STM Semaphore Open Error\n");
            exit(-1);
        }

        // When return 0 for successful access
        if (sem_wait(stm_sem) < 0){
            perror("STM Semaphore Wait Error\n");
            exit(-1);
        }


        /* Mutex Code */
        time = handle_time(stm_mem->current_hours, stm_mem->current_minutes);
        stm_mem->current_hours = *time;
        stm_mem->current_minutes = *(time + 1);
        /* End Mutex Code */

        // When return -1 then wrong issue happened
        if (sem_post(stm_sem) < 0){
            perror("STM Semaphore Release Error\n");
            exit(-1);
        }


        sem_close(stm_sem);

        usleep(TIME_STEP_INCREMENT * 1000000);

    }


    if (shmdt(stm_mem) == -1) {
        perror("STM Detached Error\n");
        exit(-1);
    }

    return(0);
}

time_t* handle_time(time_t hours, time_t minutes){
    static time_t time[2];
    minutes += 1;

    if (minutes >= 60){
        minutes = 0;
        hours += 1;
    }

    time[0] = hours;
    time[1] = minutes;

    return time;
}
