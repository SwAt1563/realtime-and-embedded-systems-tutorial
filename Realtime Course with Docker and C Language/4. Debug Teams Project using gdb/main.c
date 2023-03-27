
#include "header.h"
#include "constants.h"
#include "functions.h"


void printParent(pid_t id){
    printf("Hello i'm the main file, my id is: %d\n", id);
    printf("...\n");
}
int main(int argc, char** argv){


    int sleep = 1;
    while (sleep);



    pid_t parent_id = getpid();
    printParent(parent_id);



    pid_t teams[5];
    for (int i = 0; i < 5; ++i) {
        teams[i] = fork();
        // the team will continue from here
        if(teams[i] == -1){
            perror("No memory\n");
            exit(-5);
        } else if(teams[i] == 0){
            break;
        }else{
            printf("The main file, create the team id: %d\n", teams[i]);
        }
    }


    if(parent_id == getpid()){
        int status;
        pid_t team;
        if (team = wait(&status)){
            printf("The team %d: finished with status: %d\n", team, status);
            exit(0);
        }
    }



    pid_t players[3];
    for (int i = 0; i < 3; ++i) {
        pid_t player = fork();
        players[i] = player;
        if(player == 0){

            execlp("./player", "player", "1", NULL);
            // The player should not run this code
            perror("Exec Error\n");
            exit(-7);
        }
    }

    srand(time(NULL));   // Initialization, should only be called once.
    int r;
    r = (rand() % 3);
    printf("The team should wait the  player with the number: %d\n", r);

    int state;
    if(waitpid(players[r], &state, 0) == players[r]){
        printf("The Player with id %d is finished with status %d in the team id %d\n", players[r], state, getpid());
    }


    printf("The team %d is finished\n", getpid());



    return 0;
}