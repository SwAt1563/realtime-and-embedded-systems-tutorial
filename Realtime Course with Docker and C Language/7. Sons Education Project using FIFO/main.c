
#include "header.h"
#include "constants.h"
#include "functions.h"

char *fifo = "/tmp/cal_fifo";

int main(int argc, char** argv){




    char msg_s[BUFSIZ];

    remove(fifo);
    if (mkfifo(fifo, S_IFIFO | 0777) == -1){
        perror("Fifo Error\n");
        exit(-1);
    }


    pid_t child1 = fork();
    if ( child1 == 0){
        execlp("./child", "child", "10", NULL);
        perror("Exec Error\n");
        exit(-1);
    }

    pid_t child2 = fork();
    if ( child2 == 0){
        execlp("./child", "child", "5", NULL);
        perror("Exec Error\n");
        exit(-1);
    }


    pid_t child3 = fork();
    if ( child3 == 0){
        execlp("./child", "child", "15", NULL);
        perror("Exec Error\n");
        exit(-1);
    }


    int f = open(fifo, O_RDONLY | O_NONBLOCK);



    if ((f = open(fifo, O_WRONLY | O_NONBLOCK)) == -1){
        perror("Open Error\n");
        exit(-1);
    }else {

        strcpy(msg_s, "Find 5*5");
        if ( write(f, msg_s, sizeof(msg_s)) == -1){
            perror("Write Error\n");
            exit(-1);
        }



    }

    close(f);

    sleep(20);



    return 0;
}