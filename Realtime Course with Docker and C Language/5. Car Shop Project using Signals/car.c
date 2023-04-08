
#include "header.h"
#include "constants.h"
#include "functions.h"


int my_number;

void signal_handler(int sig);

int main(int argc, char** argv){


    if ( argc < 2){
        perror("The owner should pass the car number\n");
        exit(-1);
    }

    my_number = atoi(argv[1]);

    if(sigset(SIGINT, signal_handler) == SIG_ERR){
        perror("Signal Error\n");
        exit(-1);
    }

    pause();

    return 0;
}


void signal_handler(int sig){

    printf("The signal %d, reached\n", sig);
    printf("My car %d is sold\n", my_number);
    printf("My id is %d\n", getpid());
}