
#include "header.h"
#include "constants.h"
#include "functions.h"

char *fifo = "/tmp/cal_fifo";

int main(int argc, char** argv){


    if (argc < 2){
        perror("Args less than 2\n");
        exit(-1);
    }

    int duration_time = atoi(argv[1]);
    sleep(duration_time);

    int f;
    char msg_r[BUFSIZ] = "";



    if ((f = open(fifo, O_RDONLY | O_NONBLOCK)) == -1){
        perror("Open Error\n");
        exit(-1);
    }else {

        if ( read(f, msg_r, sizeof(msg_r)) == -1){
            perror("Read Error\n");
            exit(-1);
        }

    }

    close(f);


    if(strcmp(msg_r, "") == 0){
        printf("I'm the child with id %d, and i didn't answer the question\n", getpid());
    }else{
        printf("I'm the child with id %d, and my answer is 25\n", getpid());
    }





    return 0;
}