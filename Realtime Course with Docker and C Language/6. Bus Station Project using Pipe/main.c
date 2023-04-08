
#include "header.h"
#include "constants.h"
#include "functions.h"

/*
 *
 * Bus (T) ---> Person (R)
 * BUS (R) <--- Person (T)
 */

int main(int argc, char** argv){

    int f_des[2];
    static char message[BUFSIZ];


    if (pipe(f_des) == -1){
        perror("Pipe Error\n");
        exit(-1);
    }


    pid_t child_id = fork();

    if (child_id == -1){
        perror("Fork Error\n");
        exit(-1);
    } else if (child_id == 0){
        // PERSON
        close(f_des[1]);
        if (read(f_des[0], message, BUFSIZ) != -1){
            printf("I'm the person, the %s reached me\n", message);

        }else {
            printf("Read error\n");
        }

    }else{
        // BUS
        close(f_des[0]);
        char str[10] = "Hook";
        sleep(5);
        if (write(f_des[1], str, sizeof(str)) != -1){
            printf("The hook sent\n");
        }else{
            printf("Write error\n");
        }


    }





    return 0;
}