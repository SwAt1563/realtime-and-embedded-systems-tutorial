
#include "header.h"
#include "constants.h"
#include "functions.h"


enum SOLD {NO, YES};


int main(int argc, char** argv){


    pid_t my_id = getpid();
    readFile("arguments.txt");

    enum SOLD is_sold[NUMBER_OF_CARS];
    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        is_sold[i] = NO;
    }

    pid_t cars[NUMBER_OF_CARS];

    for (int i = 0; i < NUMBER_OF_CARS; ++i) {

        cars[i] = fork();
        if(cars[i] == 0){

            int car_number = i + 1;
            char car_name[3];
            sprintf(car_name, "%d", car_number);

            execlp("./car", "car", car_name, NULL);
            perror("Exec Error\n");
            exit(-5);
        }


        printf("The car with number: %d was created\n", cars[i]);

    }


    int read = 0;
    while (1){
        printf("We have %d cars, which car you want to buy\nif you don't want any car write -1\n", NUMBER_OF_CARS);

        scanf("%d", &read);
        if(read == -1)
            break;

        int car_id = read - 1;

        kill(cars[car_id], SIGINT);
        

        sleep(1);

    }




    printf("the program is finished\n");

    return 0;
}