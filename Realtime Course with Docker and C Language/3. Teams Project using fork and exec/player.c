
#include "header.h"
#include "constants.h"
#include "functions.h"



enum Gender{MALE = 1, FEMALE};

enum Gender player_gender;


int main( int argc, char** argv){





    if ( argc < 2){
        perror("Argc less than 2\n");
        exit(-1);
    }

    player_gender = (enum Gender) atoi(argv[1]);

    if (player_gender == MALE){
        printf("Player is Male\n");
    }

    printf("I'm the player with the id:%d\n", getpid());



    return 0;
}