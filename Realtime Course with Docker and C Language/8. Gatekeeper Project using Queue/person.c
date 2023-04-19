
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"

enum GENDER my_gender;
char my_name[20];

int main(int argc, char** argv){


    if (argc < 2){
        perror("args less than 2\n");
        exit(-1);
    }

    my_gender = (enum GENDER) atoi(argv[1]);
    strcpy(my_name, argv[2]);



    int queue = get_queue(MQ_KEY);

    MessagesQueue person_data;
    person_data.gender = my_gender;
    strcpy(person_data.name, my_name);



    if (msgsnd(queue, &person_data, sizeof(person_data), 0) == -1) {
        perror("Queue Send Error\n");
        exit(-1);
    }






    return 0;
}