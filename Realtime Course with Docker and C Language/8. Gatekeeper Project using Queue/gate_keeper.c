
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"


int main(int argc, char** argv){


    int male_hall = 0;
    int female_hall = 0;
    int x_hall = 0;

    int queue = get_queue(MQ_KEY);
    MessagesQueue person_data;

    while (1){

        if (male_hall < 2 && !check_queue_empty(queue, MALE)){

            if(msgrcv(queue, &person_data, sizeof(person_data), MALE, 0) == -1){
                perror("Queue received error\n");
                exit(-1);
            }

            male_hall++;
            printf("The size of male hall is:%d\nThe person %s Entered the hall\n", male_hall, person_data.name);


        }else if(female_hall < 2 && !check_queue_empty(queue, FEMALE)){
            if(msgrcv(queue, &person_data, sizeof(person_data), FEMALE, 0) == -1){
                perror("Queue received error\n");
                exit(-1);
            }

            female_hall++;
            printf("The size of female hall is:%d\nThe person %s Entered the hall\n", female_hall, person_data.name);
        }else if(x_hall < 1 && !check_queue_empty(queue, 0)) {
            if(msgrcv(queue, &person_data, sizeof(person_data), 0, 0) == -1){
                perror("Queue received error\n");
                exit(-1);
            }

            x_hall++;
            printf("The size of x hall is:%d\nThe person %s Entered the hall\n", x_hall, person_data.name);
        }


        if(x_hall == 1 && male_hall == 2 && female_hall == 2)
            break;



    }

    printf("The Gate Keeper Job Is Finished\n");



    return 0;
}